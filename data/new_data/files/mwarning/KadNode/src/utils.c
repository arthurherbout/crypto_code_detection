
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>

#include "main.h"
#include "log.h"
#include "conf.h"
#include "utils.h"


int hex_get_id(uint8_t id[], size_t idsize, const char query[])
{
	size_t querysize;
/*
	const char *dot;

	// Cut out first domain
	dot = strchr(query, '.');
	if (dot) {
		querysize = dot - &query[0];
	} else {
		querysize = strlen(query);
	}
*/
	querysize = strlen(query);
	if (EXIT_SUCCESS == bytes_from_base32hex(id, idsize, query, querysize)) {
		return EXIT_SUCCESS;
	}

	if (EXIT_SUCCESS == bytes_from_base16hex(id, idsize, query, querysize)) {
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

static size_t base16hex_len(size_t len)
{
	return 2 * len;
}

int bytes_from_base16hex(uint8_t dst[], size_t dstsize, const char src[], size_t srcsize)
{
	size_t i;
	size_t xv = 0;

	if (base16hex_len(dstsize) != srcsize) {
		return EXIT_FAILURE;
	}

	for (i = 0; i < srcsize; ++i) {
		const char c = src[i];
		if (c >= '0' && c <= '9') {
			xv += c - '0';
		} else if (c >= 'a' && c <= 'f') {
			xv += (c - 'a') + 10;
		} else {
			return EXIT_FAILURE;
		}

		if (i % 2) {
			dst[i / 2] = xv;
			xv = 0;
		} else {
			xv *= 16;
		}
	}

	return EXIT_SUCCESS;
}

char *bytes_to_base16hex(char dst[], size_t dstsize, const uint8_t src[], size_t srcsize)
{
	static const char hexchars[16] = "0123456789abcdef";
	size_t i;

	// + 1 for the '\0'
	if (dstsize != (base16hex_len(srcsize) + 1)) {
		return NULL;
	}

	for (i = 0; i < srcsize; ++i) {
		dst[2 * i] = hexchars[src[i] / 16];
		dst[2 * i + 1] = hexchars[src[i] % 16];
	}

	dst[2 * srcsize] = '\0';

	return dst;
}

// get length of len hex string as bytes string
// e.g.: 32 bytes need 52 characters to encode in base32hex
static size_t base32hex_len(size_t len)
{
	const size_t mod = (len % 5);
	return 8 * (len / 5) + 2 * mod - (mod > 2);
}

int bytes_from_base32hex(uint8_t dst[], size_t dstsize, const char src[], size_t srcsize)
{
	size_t processed = 0;
	unsigned char *d = dst;
	int i;
	int v;

	if (srcsize != base32hex_len(dstsize)) {
		return EXIT_FAILURE;
	}

	for (i = 0; i < srcsize; i++) {
		if (*src >= 'a' && *src <= 'v') {
			v = *src - 'a' + 10;
		} else if (*src >= '0' && *src <= '9') {
			v = *src - '0';
		} else if (*src == '=') {
			src++;
			continue;
		} else {
			return EXIT_FAILURE;
		}

		src++;

		switch (processed % 8) {
		case 0:
			if (dstsize <= 0) {
				return EXIT_FAILURE;
			}
			d[0] &= 0x07;
			d[0] |= (v << 3) & 0xF8;
			break;
		case 1:
			if (dstsize < 1) {
				return EXIT_FAILURE;
			}
			d[0] &= 0xF8;
			d[0] |= (v >> 2) & 0x07;
			if (dstsize >= 2) {
				d[1] &= 0x3F;
				d[1] |= (v << 6) & 0xC0;
			}
			break;
		case 2:
			if (dstsize < 2) {
				return EXIT_FAILURE;
			}
			d[1] &= 0xC1;
			d[1] |= (v << 1) & 0x3E;
			break;
		case 3:
			if (dstsize < 2) {
				return EXIT_FAILURE;
			}
			d[1] &= 0xFE;
			d[1] |= (v >> 4) & 0x01;
			if (dstsize >= 3) {
				d[2] &= 0x0F;
				d[2] |= (v << 4) & 0xF0;
			}
			break;
		case 4:
			if (dstsize < 3) {
				return EXIT_FAILURE;
			}
			d[2] &= 0xF0;
			d[2] |= (v >> 1) & 0x0F;
			if (dstsize >= 4) {
				d[3] &= 0x7F;
				d[3] |= (v << 7) & 0x80;
			}
			break;
		case 5:
			if (dstsize < 4) {
				return EXIT_FAILURE;
			}
			d[3] &= 0x83;
			d[3] |= (v << 2) & 0x7C;
			break;
		case 6:
			if (dstsize < 4) {
				return EXIT_FAILURE;
			}
			d[3] &= 0xFC;
			d[3] |= (v >> 3) & 0x03;
			if (dstsize >= 5) {
				d[4] &= 0x1F;
				d[4] |= (v << 5) & 0xE0;
			}
			break;
		default:
			if (dstsize < 5) {
				return EXIT_FAILURE;
			}
			d[4] &= 0xE0;
			d[4] |= v & 0x1F;
			d += 5;
			dstsize -= 5;
			break;
		}
		processed++;
	}

	return EXIT_SUCCESS;
}

char *bytes_to_base32hex(char dst[], size_t dstsize, const uint8_t *src, size_t srcsize) {
	const uint8_t *s = src;
	int byte = 0;
	uint8_t *d = (uint8_t*) dst;
	int i;

	// + 1 for the '\0'
	if (dstsize != (base32hex_len(srcsize) + 1)) {
		return NULL;
	}

	while (srcsize) {
		switch (byte) {
		case 0:
			d[0] = *s >> 3;
			d[1] = (*s & 0x07) << 2;
			break;
		case 1:
			d[1] |= (*s >> 6) & 0x03;
			d[2] = (*s >> 1) & 0x1f;
			d[3] = (*s & 0x01) << 4;
			break;
		case 2:
			d[3] |= (*s >> 4) & 0x0f;
			d[4] = (*s & 0x0f) << 1;
			break;
		case 3:
			d[4] |= (*s >> 7) & 0x01;
			d[5] = (*s >> 2) & 0x1f;
			d[6] = (*s & 0x03) << 3;
			break;
		case 4:
			d[6] |= (*s >> 5) & 0x07;
			d[7] = *s & 0x1f;
			break;
		}

		srcsize--;
		s++;
		byte++;

		if (byte == 5) {
			d += 8;
			byte = 0;
		}
	}

	d = (uint8_t*) dst;

	dstsize--;
	for (i = 0; i < dstsize; i++) {
		if (*d < 10) {
			*d = *d +'0';
		} else if (*d < 32) {
			*d = *d - 10 + 'a';
		} else {
			*d = '?';
		}
		d++;
	}

	dst[dstsize] = '\0';

	return dst;
}

// Check if a string has and extension.
// ext is epected to start with a dot.
int has_ext(const char str[], const char ext[])
{
	const char *dot = strrchr(str, '.');
	return dot && (strcmp(dot, ext) == 0);
}

/*
* Sanitize a query string.
* Convert to lowercase and remove the TLD if it matches --query-tld.
*
* example.com.p2p => example.com
* example.com => example.com
* example.p2p => example
* eXample.COM.P2P => example.com
*/
int query_sanitize(char buf[], size_t buflen, const char query[])
{
	const char *tld;
	size_t len;
	size_t i;

	len = strlen(query);

	if ((len + 1) > buflen) {
		// Output buffer too small
		return EXIT_FAILURE;
	}

	// Convert to lower case
	for (i = 0; i <= len; ++i) {
		buf[i] = tolower(query[i]);
	}

	// Remove .p2p suffix
	tld = gconf->query_tld;
	if (has_ext(buf, tld)) {
		len -= strlen(tld);
		buf[len] = '\0';
	}

	return EXIT_SUCCESS;
}

// Create a random port != 0
int port_random(void)
{
	uint16_t port;

	do {
		bytes_random((uint8_t*) &port, sizeof(port));
	} while (port == 0);

	return port;
}

// Parse a port - treats 0 as valid port
int port_parse(const char pstr[], int err)
{
	int port;
	char c;

	if (sscanf(pstr, "%d%c", &port, &c) == 1 && port >= 0 && port <= 65535) {
		return port;
	} else {
		return err;
	}
}

int port_set(IP *addr, uint16_t port)
{
	switch (addr->ss_family) {
	case AF_INET:
		((IP4 *)addr)->sin_port = htons(port);
		return EXIT_SUCCESS;
	case AF_INET6:
		((IP6 *)addr)->sin6_port = htons(port);
		return EXIT_SUCCESS;
	default:
		return EXIT_FAILURE;
	}
}

// Fill buffer with random bytes
int bytes_random(uint8_t buffer[], size_t size)
{
	int fd;
	int rc;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0) {
		log_error("Failed to open /dev/urandom");
		exit(1);
	}

	rc = read(fd, buffer, size);

	close(fd);

	return rc;
}

int id_equal(const uint8_t id1[], const uint8_t id2[])
{
	return (memcmp(id1, id2, SHA1_BIN_LENGTH) == 0);
}

const char *str_id(const uint8_t id[])
{
	static char hexbuf[SHA1_HEX_LENGTH + 1];
	return bytes_to_base16hex(hexbuf, sizeof(hexbuf), id, SHA1_BIN_LENGTH);
}

const char *str_af(int af) {
	switch (af) {
	case AF_INET:
		return "IPv4";
	case AF_INET6:
		return "IPv6";
	case AF_UNSPEC:
		return "IPv4+IPv6";
	default:
		return "<invalid>";
	}
}

const char *str_addr(const IP *addr)
{
	static char addrbuf[FULL_ADDSTRLEN];
	char buf[INET6_ADDRSTRLEN];
	const char *fmt;
	int port;

	switch (addr->ss_family) {
	case AF_INET6:
		port = ((IP6 *)addr)->sin6_port;
		inet_ntop(AF_INET6, &((IP6 *)addr)->sin6_addr, buf, sizeof(buf));
		fmt = "[%s]:%d";
		break;
	case AF_INET:
		port = ((IP4 *)addr)->sin_port;
		inet_ntop(AF_INET, &((IP4 *)addr)->sin_addr, buf, sizeof(buf));
		fmt = "%s:%d";
		break;
	default:
		return "<invalid address>";
	}

	sprintf(addrbuf, fmt, buf, ntohs(port));

	return addrbuf;
}

int addr_is_localhost(const IP *addr)
{
	// 127.0.0.1
	const uint32_t inaddr_loopback = htonl(INADDR_LOOPBACK);

	switch (addr->ss_family) {
	case AF_INET:
		return (memcmp(&((IP4 *)addr)->sin_addr, &inaddr_loopback, 4) == 0);
	case AF_INET6:
		return (memcmp(&((IP6 *)addr)->sin6_addr, &in6addr_loopback, 16) == 0);
	default:
		return 0;
	}
}

int addr_is_multicast(const IP *addr)
{
	switch (addr->ss_family) {
	case AF_INET:
		return IN_MULTICAST(ntohl(((IP4*) addr)->sin_addr.s_addr));
	case AF_INET6:
		return IN6_IS_ADDR_MULTICAST(&((IP6*) addr)->sin6_addr);
	default:
		return 0;
	}
}

int addr_port(const IP *addr)
{
	switch (addr->ss_family) {
	case AF_INET:
		return ntohs(((IP4 *)addr)->sin_port);
	case AF_INET6:
		return ntohs(((IP6 *)addr)->sin6_port);
	default:
		return 0;
	}
}

int addr_len(const IP *addr)
{
	switch (addr->ss_family) {
	case AF_INET:
		return sizeof(IP4);
	case AF_INET6:
		return sizeof(IP6);
	default:
		return 0;
	}
}

/*
* Parse/Resolve an IP address.
* The port must be specified separately.
*/
int addr_parse(IP *addr, const char addr_str[], const char port_str[], int af)
{
	struct addrinfo hints;
	struct addrinfo *info = NULL;
	struct addrinfo *p = NULL;

	memset(&hints, '\0', sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_NUMERICHOST;
	hints.ai_family = af;

	if (getaddrinfo(addr_str, port_str, &hints, &info) != 0) {
		return -2;
	}

	p = info;
	while (p != NULL) {
		if (p->ai_family == AF_INET6) {
			memcpy(addr, p->ai_addr, sizeof(IP6));
			freeaddrinfo(info);
			return 0;
		}
		if (p->ai_family == AF_INET) {
			memcpy(addr, p->ai_addr, sizeof(IP4));
			freeaddrinfo(info);
			return 0;
		}
		p = p->ai_next;
	}

	freeaddrinfo(info);

	return -3;
}

/*
* Parse/Resolve various string representations of
* IPv4/IPv6 addresses and optional port.
* An address can also be a domain name.
* A port can also be a service	(e.g. 'www').
*
* "<address>"
* "<ipv4_address>:<port>"
* "[<address>]"
* "[<address>]:<port>"
*/
int addr_parse_full(IP *addr, const char full_addr_str[], const char default_port[], int af)
{
	char addr_buf[256];
	char *addr_beg;
	char *addr_tmp;
	char *last_colon;
	const char *addr_str = NULL;
	const char *port_str = NULL;
	size_t len;

	len = strlen(full_addr_str);
	if (len >= (sizeof(addr_buf) - 1)) {
		// address too long
		return -1;
	} else {
		addr_beg = addr_buf;
	}

	memset(addr_buf, '\0', sizeof(addr_buf));
	memcpy(addr_buf, full_addr_str, len);

	last_colon = strrchr(addr_buf, ':');

	if (addr_beg[0] == '[') {
		// [<addr>] or [<addr>]:<port>
		addr_tmp = strrchr(addr_beg, ']');

		if (addr_tmp == NULL) {
			// broken format
			return -1;
		}

		*addr_tmp = '\0';
		addr_str = addr_beg + 1;

		if (*(addr_tmp+1) == '\0') {
			port_str = default_port;
		} else if (*(addr_tmp+1) == ':') {
			port_str = addr_tmp + 2;
		} else {
			// port expected
			return -1;
		}
	} else if (last_colon && last_colon == strchr(addr_buf, ':')) {
		// <non-ipv6-addr>:<port>
		addr_tmp = last_colon;
		if (addr_tmp) {
			*addr_tmp = '\0';
			addr_str = addr_buf;
			port_str = addr_tmp+1;
		} else {
			addr_str = addr_buf;
			port_str = default_port;
		}
	} else {
		// <addr>
		addr_str = addr_buf;
		port_str = default_port;
	}

	return addr_parse(addr, addr_str, port_str, af);
}

// Compare two ip addresses, ignore port
int addr_equal(const IP *addr1, const IP *addr2)
{
	if (addr1->ss_family != addr2->ss_family) {
		return 0;
	} else if (addr1->ss_family == AF_INET) {
		return 0 == memcmp(&((IP4 *)addr1)->sin_addr, &((IP4 *)addr2)->sin_addr, 4);
	} else if (addr1->ss_family == AF_INET6) {
		return 0 == memcmp(&((IP6 *)addr1)->sin6_addr, &((IP6 *)addr2)->sin6_addr, 16);
	} else {
		return 0;
	}
}

int socket_addr(int sock, IP *addr)
{
	socklen_t len = sizeof(IP);
	return getsockname(sock, (struct sockaddr *) addr, &len);
}

time_t time_add_secs(uint32_t seconds)
{
	return gconf->time_now + seconds;
}

time_t time_add_mins(uint32_t minutes)
{
	return gconf->time_now + (60 * minutes);
}

time_t time_add_hours(uint32_t hours)
{
	return gconf->time_now + (60 * 60 * hours);
}
