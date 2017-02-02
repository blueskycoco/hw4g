#ifndef _HEADER_H_
#define _HEADER_H_

#define H264        96
typedef struct rtp_header {
    /* little-endian */
    /* byte 0 */
    uint8_t csrc_len:       4;  /* bit: 0~3 */
    uint8_t extension:      1;  /* bit: 4 */
    uint8_t padding:        1;  /* bit: 5*/
    uint8_t version:        2;  /* bit: 6~7 */
    /* byte 1 */
    uint8_t payload_type:   7;  /* bit: 0~6 */
    uint8_t marker:         1;  /* bit: 7 */
    /* bytes 2, 3 */
    uint16_t seq_no;            
    /* bytes 4-7 */
    uint32_t timestamp;        
    /* bytes 8-11 */
    uint32_t ssrc;
} __attribute__ ((packed)) rtp_header_t; /* 12 bytes */
typedef struct nalu_header {
    /* byte 0 */
	uint8_t type:   5;  /* bit: 0~4 */
    uint8_t nri:    2;  /* bit: 5~6 */
	uint8_t f:      1;  /* bit: 7 */
} __attribute__ ((packed)) nalu_header_t; /* 1 bytes */
typedef struct fu_indicator {
    /* byte 0 */
    uint8_t type:   5;
	uint8_t nri:    2; 
	uint8_t f:      1;    
} __attribute__ ((packed)) fu_indicator_t; /* 1 bytes */
typedef struct fu_header {
    /* byte 0 */
    uint8_t type:   5;
	uint8_t r:      1;
	uint8_t e:      1;
	uint8_t s:      1;    
} __attribute__ ((packed)) fu_header_t; /* 1 bytes */
#endif
