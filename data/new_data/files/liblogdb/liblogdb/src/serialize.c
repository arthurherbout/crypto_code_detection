/* Copyright 2012 exMULTI, Inc.
 * Distributed under the MIT/X11 software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 */

#include <logdb/cstr.h>
#include <logdb/serialize.h>

#include <string.h>

void ser_bytes(cstring* s, const void* p, size_t len)
{
    cstr_append_buf(s, p, len);
}

void ser_u16(cstring* s, uint16_t v_)
{
    uint16_t v = htole16(v_);
    cstr_append_buf(s, &v, sizeof(v));
}

void ser_u32(cstring* s, uint32_t v_)
{
    uint32_t v = htole32(v_);
    cstr_append_buf(s, &v, sizeof(v));
}

void ser_u64(cstring* s, uint64_t v_)
{
    uint64_t v = htole64(v_);
    cstr_append_buf(s, &v, sizeof(v));
}

void ser_varlen(cstring* s, uint32_t vlen)
{
    unsigned char c;

    if (vlen < 253) {
        c = vlen;
        ser_bytes(s, &c, 1);
    }

    else if (vlen < 0x10000) {
        c = 253;
        ser_bytes(s, &c, 1);
        ser_u16(s, (uint16_t)vlen);
    }

    else {
        c = 254;
        ser_bytes(s, &c, 1);
        ser_u32(s, vlen);
    }

    /* u64 case intentionally not implemented */
}

void ser_str(cstring* s, const char* s_in, size_t maxlen)
{
    size_t slen = strnlen(s_in, maxlen);

    ser_varlen(s, slen);
    ser_bytes(s, s_in, slen);
}

void ser_varstr(cstring* s, cstring* s_in)
{
    if (!s_in || !s_in->len) {
        ser_varlen(s, 0);
        return;
    }

    ser_varlen(s, s_in->len);
    ser_bytes(s, s_in->str, s_in->len);
}

int deser_bytes(void* po, struct const_buffer* buf, size_t len)
{
    char *p;
    if (buf->len < len)
        return false;

    memcpy(po, buf->p, len);
    p = (char *)buf->p;
    p += len;
    buf->p = p;
    buf->len -= len;

    return true;
}

int deser_u16(uint16_t* vo, struct const_buffer* buf)
{
    uint16_t v;

    if (!deser_bytes(&v, buf, sizeof(v)))
        return false;

    *vo = le16toh(v);
    return true;
}

int deser_u32(uint32_t* vo, struct const_buffer* buf)
{
    uint32_t v;

    if (!deser_bytes(&v, buf, sizeof(v)))
        return false;

    *vo = le32toh(v);
    return true;
}

int deser_u64(uint64_t* vo, struct const_buffer* buf)
{
    uint64_t v;

    if (!deser_bytes(&v, buf, sizeof(v)))
        return false;

    *vo = le64toh(v);
    return true;
}

int deser_varlen(uint32_t* lo, struct const_buffer* buf)
{
    uint32_t len;

    unsigned char c;
    if (!deser_bytes(&c, buf, 1))
        return false;

    if (c == 253) {
        uint16_t v16;
        if (!deser_u16(&v16, buf))
            return false;
        len = v16;
    } else if (c == 254) {
        uint32_t v32;
        if (!deser_u32(&v32, buf))
            return false;
        len = v32;
    } else if (c == 255) {
        uint64_t v64;
        if (!deser_u64(&v64, buf))
            return false;
        len = (uint32_t)v64; /* WARNING: truncate */
    } else
        len = c;

    *lo = len;
    return true;
}

int deser_varlen_file(uint32_t* lo, FILE *file, uint8_t *rawdata, size_t *buflen_inout)
{
    uint32_t len;
    struct const_buffer buf;
    unsigned char c;
    const unsigned char bufp[sizeof(uint64_t)];

    /* check min size of the buffer */
    if (*buflen_inout < sizeof(len))
        return false;

    if (fread(&c, 1, 1, file) != 1)
        return false;

    rawdata[0] = c;
    *buflen_inout = 1;

    buf.p = (void *)bufp;
    buf.len = sizeof(uint64_t);

    if (c == 253) {
        uint16_t v16;
        if (fread((void *)buf.p, 1, sizeof(v16), file) != sizeof(v16))
            return false;
        memcpy(rawdata+1, buf.p, sizeof(v16));
        *buflen_inout += sizeof(v16);
        if (!deser_u16(&v16, &buf))
            return false;
        len = v16;
    } else if (c == 254) {
        uint32_t v32;
        if (fread((void *)buf.p, 1, sizeof(v32), file) != sizeof(v32))
            return false;
        memcpy(rawdata+1, buf.p, sizeof(v32));
        *buflen_inout += sizeof(v32);
        if (!deser_u32(&v32, &buf))
            return false;
        len = v32;
    } else if (c == 255) {
        uint64_t v64;
        if (fread((void *)buf.p, 1, sizeof(v64), file) != sizeof(v64))
            return false;
        memcpy(rawdata+1, buf.p, sizeof(uint32_t)); /* warning, truncate! */
        *buflen_inout += sizeof(uint32_t);
        if (!deser_u64(&v64, &buf))
            return false;
        len = (uint32_t)v64; /* WARNING: truncate */
    } else
        len = c;

    *lo = len;
    return true;
}

