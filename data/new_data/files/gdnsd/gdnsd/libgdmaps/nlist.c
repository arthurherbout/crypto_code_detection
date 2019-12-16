/* Copyright © 2012 Brandon L Black <blblack@gmail.com>
 *
 * This file is part of gdnsd.
 *
 * gdnsd-plugin-geoip is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gdnsd-plugin-geoip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gdnsd.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <config.h>
#include "nlist.h"

#include "dclists.h"

#include <gdnsd/alloc.h>
#include <gdnsd/log.h>
#include <gdnsd/misc.h>

#include <string.h>
#include <stdlib.h>

#define NLIST_INITSIZE 64

typedef struct {
    uint8_t ipv6[16];
    unsigned mask;
    unsigned dclist;
} net_t;

struct nlist {
    net_t* nets;
    char* map_name;
    unsigned alloc;
    unsigned count;
    bool normalized;
};

nlist_t* nlist_new(const char* map_name, const bool pre_norm)
{
    nlist_t* nl = xmalloc(sizeof(*nl));
    nl->nets = xmalloc_n(NLIST_INITSIZE, sizeof(*nl->nets));
    nl->map_name = xstrdup(map_name);
    nl->alloc = NLIST_INITSIZE;
    nl->count = 0;
    nl->normalized = pre_norm;
    return nl;
}

// only used for normalization assertions in debug builds...
F_UNUSED F_NONNULL
static nlist_t* nlist_clone(const nlist_t* nl)
{
    nlist_t* nlc = xmalloc(sizeof(*nlc));
    nlc->map_name = xstrdup(nl->map_name);
    nlc->alloc = nl->alloc;
    nlc->count = nl->count;
    nlc->normalized = nl->normalized;
    nlc->nets = xmalloc_n(nlc->alloc, sizeof(*nlc->nets));
    memcpy(nlc->nets, nl->nets, sizeof(*nlc->nets) * nlc->count);
    return nlc;
}

void nlist_debug_dump(const nlist_t* nl)
{
    log_debug(" --- nlist debug on %s --- ", nl->map_name);
    for (unsigned i = 0; i < nl->count; i++)
        log_debug("   %s/%u -> %u", logf_ipv6(nl->nets[i].ipv6), nl->nets[i].mask, nl->nets[i].dclist);
}

void nlist_destroy(nlist_t* nl)
{
    free(nl->map_name);
    free(nl->nets);
    free(nl);
}

#ifndef NDEBUG
F_NONNULL
static void assert_clear_mask_bits(uint8_t* ipv6, const unsigned mask)
{
    gdnsd_assert(mask < 129);

    if (likely(mask)) {
        const unsigned revmask = 128 - mask;
        const unsigned byte_mask = ~(0xFFU << (revmask & 7)) & 0xFF;
        unsigned bbyte = 15 - (revmask >> 3);

        gdnsd_assert(!(ipv6[bbyte] & byte_mask));
        while (++bbyte < 16)
            gdnsd_assert(!ipv6[bbyte]);
    } else {
        gdnsd_assert(!memcmp(ipv6, &ip6_zero.s6_addr, 16));
    }
}
#else
#define assert_clear_mask_bits(x, y)
#endif

F_NONNULL
static void clear_mask_bits(const char* map_name, uint8_t* ipv6, const unsigned mask)
{
    gdnsd_assert(mask < 129);

    bool maskbad = false;

    if (likely(mask)) {
        const unsigned revmask = 128 - mask;
        const unsigned byte_mask = ~(0xFFU << (revmask & 7)) & 0xFF;
        unsigned bbyte = 15 - (revmask >> 3);

        if (ipv6[bbyte] & byte_mask) {
            maskbad = true;
            ipv6[bbyte] &= ~byte_mask;
        }

        while (++bbyte < 16) {
            if (ipv6[bbyte]) {
                maskbad = true;
                ipv6[bbyte] = 0;
            }
        }
    } else if (memcmp(ipv6, &ip6_zero.s6_addr, 16)) {
        maskbad = true;
        memset(ipv6, 0, 16);
    }

    if (maskbad)
        log_warn("plugin_geoip: map '%s': input network %s/%u had illegal bits beyond mask, which were cleared", map_name, logf_ipv6(ipv6), mask);
}

// Sort an array of net_t.  Sort prefers
//   lowest network number, smallest mask.
F_NONNULL F_PURE
static int net_sorter(const void* a_void, const void* b_void)
{
    const net_t* a = a_void;
    const net_t* b = b_void;
    int rv = memcmp(a->ipv6, b->ipv6, 16);
    if (!rv)
        rv = (int)a->mask - (int)b->mask;
    return rv;
}

F_NONNULL F_PURE
static bool masked_net_eq(const uint8_t* v6a, const uint8_t* v6b, const unsigned mask)
{
    gdnsd_assert(mask < 128U); // 2x128 would call here w/ 127...

    const unsigned bytes = mask >> 3;
    gdnsd_assert(bytes < 16U);

    const unsigned bytemask = (0xFF00U >> (mask & 7)) & 0xFF;
    return !memcmp(v6a, v6b, bytes)
           && (v6a[bytes] & bytemask) == (v6b[bytes] & bytemask);
}

F_NONNULL F_PURE
static bool mergeable_nets(const net_t* na, const net_t* nb)
{
    bool rv = false;
    if (na->dclist == nb->dclist) {
        if (na->mask == nb->mask)
            rv = masked_net_eq(na->ipv6, nb->ipv6, na->mask - 1);
        else if (na->mask < nb->mask)
            rv = masked_net_eq(na->ipv6, nb->ipv6, na->mask);
    }
    return rv;
}

void nlist_append(nlist_t* nl, const uint8_t* ipv6, const unsigned mask, const unsigned dclist)
{
    if (unlikely(nl->count == nl->alloc)) {
        nl->alloc <<= 1U;
        nl->nets = xrealloc_n(nl->nets, nl->alloc, sizeof(*nl->nets));
    }
    net_t* this_net = &nl->nets[nl->count++];
    memcpy(this_net->ipv6, ipv6, 16U);
    this_net->mask = mask;
    this_net->dclist = dclist;

    // In the pre-norm case, we can keep the list in fully-normalized
    //   form as we go by doing this merge op after each append and
    //   keeping the list minimized.  What we're catching here is adjacent
    //   networks which share a dclist and mask and are thus mergeable,
    //   and we do so by deleting the most-recently added one and decrementing
    //   the subnet mask of the older one.
    // Because this is happening back-to-front after each append, there's
    //   no need to create (or later deal with) holes in the array.
    if (nl->normalized) {
        assert_clear_mask_bits(this_net->ipv6, mask);
        unsigned idx = nl->count;
        while (--idx > 0) {
            net_t* nb = &nl->nets[idx];
            net_t* na = &nl->nets[idx - 1];
            if (mergeable_nets(na, nb)) {
                if (na->mask == nb->mask)
                    na->mask--;
                nl->count--;
            } else {
                break;
            }
        }
    } else {
        // for raw input, just correct any netmask errors as we insert,
        //   as these will screw up later sorting for normalization
        clear_mask_bits(nl->map_name, this_net->ipv6, mask);
    }
}

F_NONNULL F_PURE
static bool net_eq(const net_t* na, const net_t* nb)
{
    return na->mask == nb->mask && !memcmp(na->ipv6, nb->ipv6, 16);
}

// do a single pass of forward-normalization
//   on a sorted nlist, then sort the result.
F_NONNULL
static bool nlist_normalize_1pass(nlist_t* nl)
{
    gdnsd_assert(nl->count);

    bool rv = false;

    const unsigned oldcount = nl->count;
    unsigned newcount = nl->count;
    unsigned i = 0;
    while (i < oldcount) {
        net_t* na = &nl->nets[i];
        unsigned j = i + 1;
        while (j < oldcount) {
            net_t* nb = &nl->nets[j];
            if (net_eq(na, nb)) { // net+mask match, dclist may or may not match
                if (na->dclist != nb->dclist)
                    log_warn("plugin_geoip: map '%s' nets: Exact duplicate networks with conflicting dclists at %s/%u", nl->map_name, logf_ipv6(na->ipv6), na->mask);
            } else if (mergeable_nets(na, nb)) { // dclists match, nets adjacent (masks equal) or subnet-of
                if (na->mask == nb->mask)
                    na->mask--;
            } else {
                break;
            }
            nb->mask = 0xFFFF; // illegally-huge, to sort deletes later
            memset(nb->ipv6, 0xFF, 16); // all-1's, also for sort...
            newcount--;
            j++;
        }
        i = j;
    }

    if (newcount != oldcount) { // merges happened above
        // the "deleted" entries have all-1's IPs and >legal masks, so they
        //   sort to the end...
        qsort(nl->nets, oldcount, sizeof(*nl->nets), net_sorter);

        // reset the count to ignore the deleted entries at the end
        nl->count = newcount;

        // signal need for another pass
        rv = true;
    }

    return rv;
}

F_NONNULL
static void nlist_normalize(nlist_t* nl, const bool post_merge)
{
    if (nl->count) {
        // initial sort, unless already sorted by the merge process
        if (!post_merge)
            qsort(nl->nets, nl->count, sizeof(*nl->nets), net_sorter);

        // iterate merge+sort passes until no further merges are found
        while (nlist_normalize_1pass(nl))
            ; // empty

        // optimize storage space
        if (nl->count != nl->alloc) {
            gdnsd_assert(nl->count < nl->alloc);
            nl->alloc = nl->count;
            nl->nets = xrealloc_n(nl->nets, nl->alloc, sizeof(*nl->nets));
        }
    }

    nl->normalized = true;
}

F_NONNULL
void nlist_finish(nlist_t* nl)
{
    if (nl->normalized) {
#ifndef NDEBUG
        // assert normalization in debug builds via clone->normalize->compare
        nlist_t* nlc = nlist_clone(nl);
        nlist_normalize(nlc, false);
        gdnsd_assert(nlc->count == nl->count);
        gdnsd_assert(!memcmp(nlc->nets, nl->nets, sizeof(*nlc->nets) * nlc->count));
        nlist_destroy(nlc);
#endif
    } else {
        nlist_normalize(nl, false);
    }
}

F_NONNULL F_PURE
static bool net_subnet_of(const net_t* sub, const net_t* super)
{
    gdnsd_assert(sub->mask < 129);
    gdnsd_assert(super->mask < 129);

    bool rv = false;
    if (sub->mask >= super->mask) {
        const unsigned wbyte = (super->mask >> 3);
        const unsigned byte_mask = (0xFFU << (8U - (super->mask & 7))) & 0xFF;
        if (!memcmp(sub->ipv6, super->ipv6, wbyte)
                && (wbyte == 16 || (super->ipv6[wbyte] & byte_mask) == (sub->ipv6[wbyte] & byte_mask)))
            rv = true;
    }

    return rv;
}

F_NONNULL F_RETNN
static nlist_t* nlist_merge(const nlist_t* nl_a, const nlist_t* nl_b)
{
    gdnsd_assert(nl_a->normalized);
    gdnsd_assert(nl_b->normalized);

    nlist_t* merged = nlist_new(nl_a->map_name, false);

    const net_t* n_a = &nl_a->nets[0];
    const net_t* n_b = &nl_b->nets[0];
    const net_t* end_a = &nl_a->nets[nl_a->count];
    const net_t* end_b = &nl_b->nets[nl_b->count];

    while (n_a < end_a && n_b < end_b) {
        if (net_sorter(n_a, n_b) < 0) {
            // n_a < n_b
            //   therefore n_a is a supernet of the next n_b,
            //   or an unrelated predecessor, copy it...
            nlist_append(merged, n_a->ipv6, n_a->mask, n_a->dclist);
            n_a++;
        } else { // n_a >= n_b
            nlist_append(merged, n_b->ipv6, n_b->mask, n_b->dclist);
            // this is where we skip networks from the first list
            //   that are effectively masked out by entries in the second
            while (n_a < end_a && net_subnet_of(n_a, n_b))
                n_a++;
            n_b++;
        }
    }

    // Usually only one of the lists will have remaining entries,
    //   which should be copyable.  Rarely, both will already be finished.
    while (n_b < end_b) {
        nlist_append(merged, n_b->ipv6, n_b->mask, n_b->dclist);
        n_b++;
    }
    while (n_a < end_a) {
        nlist_append(merged, n_a->ipv6, n_a->mask, n_a->dclist);
        n_a++;
    }

    nlist_normalize(merged, true);
    return merged;
}

F_NONNULL
static unsigned nxt_rec(const net_t** nl, const net_t* const nl_end, ntree_t* nt, net_t tree_net);

F_NONNULL
static void nxt_rec_dir(const net_t** nlp, const net_t* const nl_end, ntree_t* nt, net_t tree_net, const unsigned nt_idx, const bool direction)
{
    gdnsd_assert(tree_net.mask < 129 && tree_net.mask > 0);

    const net_t* nl = *nlp;
    unsigned cnode;

    // If items remain in the list, and the next list item
    //   is a subnet of (including exact match for) the current
    //   ntree node...
    if (nl < nl_end && net_subnet_of(nl, &tree_net)) {
        // exact match, consume...
        if (tree_net.mask == nl->mask) {
            (*nlp)++; // consume *nlp and move to next
            // need to pre-check for a deeper subnet next in the list.
            // We use the consumed entry as the new default and keep recursing
            //   if deeper subnets exist.  If they don't, we assign and end recursion...
            const net_t* nl_next = *nlp;
            if (nl_next < nl_end && net_subnet_of(nl_next, nl)) {
                tree_net.dclist = nl->dclist;
                cnode = nxt_rec(nlp, nl_end, nt, tree_net);
            } else {
                cnode = NN_SET_DCLIST(nl->dclist);
            }
        } else {
            // Not an exact match, so just keep recursing towards such a match...
            cnode = nxt_rec(nlp, nl_end, nt, tree_net);
        }
    } else {
        // list item isn't a subnet of the current tree node, and due to our
        //   normalization that means there are no such list items remaining,
        //   so terminate the recursion with the current default dclist.
        cnode = NN_SET_DCLIST(tree_net.dclist);
    }

    // store direct or recursed result.  Note we have to wait until
    //   here to deref nt->store[nt_idx] because recursion could
    //   re-allocate nt->store[] during nxt_rec()'s ntree_add_node() call.
    if (direction)
        nt->store[nt_idx].one = cnode;
    else
        nt->store[nt_idx].zero = cnode;
}

F_NONNULL
static unsigned nxt_rec(const net_t** nl, const net_t* const nl_end, ntree_t* nt, net_t tree_net)
{
    gdnsd_assert(tree_net.mask < 128);
    tree_net.mask++; // now mask for zero/one stubs

    const unsigned nt_idx = ntree_add_node(nt);
    nxt_rec_dir(nl, nl_end, nt, tree_net, nt_idx, false);
    SETBIT_v6(tree_net.ipv6, tree_net.mask - 1);
    nxt_rec_dir(nl, nl_end, nt, tree_net, nt_idx, true);

    unsigned rv = nt_idx;

    // catch missed optimizations during final translation
    if (unlikely(nt->store[nt_idx].zero == nt->store[nt_idx].one) && likely(nt_idx > 0)) {
        nt->count--; // delete the just-added node
        rv = nt->store[nt_idx].zero;
    }

    return rv;
}

ntree_t* nlist_xlate_tree(const nlist_t* nl)
{
    gdnsd_assert(nl->normalized);

    ntree_t* nt = ntree_new();
    const net_t* nlnet = &nl->nets[0];
    const net_t* const nlnet_end = &nl->nets[nl->count];
    net_t tree_net = {
        .ipv6 = { 0 },
        .mask = 0,
        .dclist = 0
    };

    // Special-case: if a list entry for ::/0 exists, it will
    //   be first in the list, and it needs to be skipped
    //   over (with its dclist as the new default) before
    //   recursing (because ::/0 is the first node of the
    //   tree itself).
    if (nl->count && !nl->nets[0].mask) {
        tree_net.dclist = nl->nets[0].dclist;
        nlnet++;
    }

    // recursively build the tree from the list
    nxt_rec(&nlnet, nlnet_end, nt, tree_net);

    // assert that the whole list was consumed
    gdnsd_assert(nlnet == nlnet_end);

    // finalize the tree
    ntree_finish(nt);

    // make sure all our logic worked out sanely
    ntree_assert_optimal(nt);

    return nt;
}

ntree_t* nlist_merge2_tree(const nlist_t* nl_a, const nlist_t* nl_b)
{
    nlist_t* merged = nlist_merge(nl_a, nl_b);
    ntree_t* rv = nlist_xlate_tree(merged);
    nlist_destroy(merged);
    return rv;
}
