/*
  The JTAG Whisperer: An Arduino library for JTAG.

  By Mike Tsao <http://github.com/sowbug>.

  Copyright © 2012 Mike Tsao. Use, modification, and distribution are
  subject to the BSD-style license as described in the accompanying
  LICENSE file.

  See README for complete attributions.
*/

#ifndef INCLUDE_JTAG_WHISPERER_JTAG_WHISPERER_H
#define INCLUDE_JTAG_WHISPERER_JTAG_WHISPERER_H

#include <BitTwiddler.h>
#include <SerialComm.h>
#include <Utilities.h>
#include <avr/io.h>

// These tables and the code that uses them are taken from
// https://github.com/ben0109/XSVF-Player/.
static const uint8_t tms_transitions[] = {
  0x01, /* STATE_TLR    */
  0x21, /* STATE_RTI    */
  0x93, /* STATE_SELECT_DR_SCAN */
  0x54, /* STATE_CAPTURE_DR */
  0x54, /* STATE_SHIFT_DR */
  0x86, /* STATE_EXIT1_DR */
  0x76, /* STATE_PAUSE_DR */
  0x84, /* STATE_EXIT2_DR */
  0x21, /* STATE_UPDATE_DR  */
  0x0a, /* STATE_SELECT_IR_SCAN */
  0xcb, /* STATE_CAPTURE_IR */
  0xcb, /* STATE_SHIFT_IR */
  0xfd, /* STATE_EXIT1_IR */
  0xed, /* STATE_PAUSE_IR */
  0xfb, /* STATE_EXIT2_IR */
  0x21, /* STATE_UPDATE_IR  */
};

static const uint16_t tms_map[] = {
  0x0000, /* STATE_TLR    */
  0xfffd, /* STATE_RTI    */
  0xfe03, /* STATE_SELECT_DR_SCAN */
  0xffe7, /* STATE_CAPTURE_DR */
  0xffef, /* STATE_SHIFT_DR */
  0xff0f, /* STATE_EXIT1_DR */
  0xffbf, /* STATE_PAUSE_DR */
  0xff0f, /* STATE_EXIT2_DR */
  0xfefd, /* STATE_UPDATE_DR  */
  0x01ff, /* STATE_SELECT_IR_SCAN */
  0xf3ff, /* STATE_CAPTURE_IR */
  0xf7ff, /* STATE_SHIFT_IR */
  0x87ff, /* STATE_EXIT1_IR */
  0xdfff, /* STATE_PAUSE_IR */
  0x87ff, /* STATE_EXIT2_IR */
  0x7ffd, /* STATE_UPDATE_IR  */
};

class JTAGWhisperer {
 public:
 JTAGWhisperer(SerialComm& serial_comm, BitTwiddler& twiddler)
   : serial_comm_(serial_comm),
    twiddler_(twiddler),
    current_state_(-1),
    sdrsize_bits_(0),
    sdrsize_bytes_(0),
    repeat_(32),
    runtest_(0),
    reached_xcomplete_(false) {
  }

  bool reached_xcomplete() { return reached_xcomplete_; }

  uint8_t read_next_instruction() {
    bp_ = instruction_buffer_;
    uint8_t instruction = serial_comm_.GetNextByte();
    switch (instruction) {
    case XCOMPLETE:
      break;
    case XTDOMASK:
      get_next_bytes_from_stream(sdrsize_bytes_);
      break;
    case XSIR: {
      uint8_t length = serial_comm_.GetNextByte();
      *bp_++ = length;
      get_next_bytes_from_stream(BYTES(length));
      break;
    }
    case XSDR:
    case XSDRB:
    case XSDRC:
    case XSDRE:
      get_next_bytes_from_stream(sdrsize_bytes_);
      break;
    case XRUNTEST:
      get_next_bytes_from_stream(4);
      break;
    case XREPEAT:
      *bp_++ = serial_comm_.GetNextByte();
      break;
    case XSDRSIZE:
      get_next_bytes_from_stream(4);
      break;
    case XSDRTDO:
    case XSDRTDOB:
    case XSDRTDOC:
    case XSDRTDOE:
    case XSETSDRMASKS:
      get_next_bytes_from_stream(sdrsize_bytes_ + sdrsize_bytes_);
      break;
    case XSTATE:
      *bp_++ = serial_comm_.GetNextByte();
      break;
    case XWAIT:
      get_next_bytes_from_stream(6);
      break;
    case XSDRINC:
    default:
      serial_comm_.Important("Unexpected instruction %d", instruction);
      break;
    }
    return instruction;
  }

  bool handle_instruction(uint8_t instruction) {
    bp_ = instruction_buffer_;

    serial_comm_.Debug("Handling %s", instruction_name(instruction));
    switch (instruction) {
      HANDLE(XCOMPLETE);
      HANDLE(XTDOMASK);
      HANDLE(XSIR);
      HANDLE(XSDR);
      HANDLE(XRUNTEST);
      HANDLE(XREPEAT);
      HANDLE(XSDRSIZE);
      HANDLE(XSDRTDO);
      HANDLE(XSTATE);
      HANDLE(XWAIT);
    default:
      serial_comm_.Debug("Unimplemented instruction: %d", instruction);
      return false;
    }
  }

 private:
  enum {
    XCOMPLETE = 0,
    XTDOMASK,
    XSIR,
    XSDR,
    XRUNTEST,
    XRESERVED_5,
    XRESERVED_6,
    XREPEAT,
    XSDRSIZE,
    XSDRTDO,
    XSETSDRMASKS,
    XSDRINC,
    XSDRB,
    XSDRC,
    XSDRE,
    XSDRTDOB,
    XSDRTDOC,
    XSDRTDOE,
    XSTATE,
    XENDIR,
    XENDDR,
    XSIR2,
    XCOMMENT,
    XWAIT
  };
  const char *instruction_name(uint8_t instruction) {
    switch (instruction) {
      NAME_FOR(XCOMPLETE);
      NAME_FOR(XTDOMASK);
      NAME_FOR(XSIR);
      NAME_FOR(XSDR);
      NAME_FOR(XRUNTEST);
      NAME_FOR(XREPEAT);
      NAME_FOR(XSDRSIZE);
      NAME_FOR(XSDRTDO);
      NAME_FOR(XSETSDRMASKS);
      NAME_FOR(XSDRINC);
      NAME_FOR(XSDRB);
      NAME_FOR(XSDRC);
      NAME_FOR(XSDRE);
      NAME_FOR(XSDRTDOB);
      NAME_FOR(XSDRTDOC);
      NAME_FOR(XSDRTDOE);
      NAME_FOR(XSTATE);
      NAME_FOR(XENDIR);
      NAME_FOR(XENDDR);
      NAME_FOR(XSIR2);
      NAME_FOR(XCOMMENT);
      NAME_FOR(XWAIT);
    default:
      return "XWTF";
    }
  }

  enum {
    STATE_TLR,
    STATE_RTI,
    STATE_SELECT_DR_SCAN,
    STATE_CAPTURE_DR,
    STATE_SHIFT_DR,
    STATE_EXIT1_DR,
    STATE_PAUSE_DR,
    STATE_EXIT2_DR,
    STATE_UPDATE_DR,
    STATE_SELECT_IR_SCAN,
    STATE_CAPTURE_IR,
    STATE_SHIFT_IR,
    STATE_EXIT1_IR,
    STATE_PAUSE_IR,
    STATE_EXIT2_IR,
    STATE_UPDATE_IR,
  };
  const char *state_name(uint8_t state) {
    switch (state) {
      NAME_FOR(STATE_TLR);
      NAME_FOR(STATE_RTI);
      NAME_FOR(STATE_SELECT_DR_SCAN);
      NAME_FOR(STATE_CAPTURE_DR);
      NAME_FOR(STATE_SHIFT_DR);
      NAME_FOR(STATE_EXIT1_DR);
      NAME_FOR(STATE_PAUSE_DR);
      NAME_FOR(STATE_EXIT2_DR);
      NAME_FOR(STATE_UPDATE_DR);
      NAME_FOR(STATE_SELECT_IR_SCAN);
      NAME_FOR(STATE_CAPTURE_IR);
      NAME_FOR(STATE_SHIFT_IR);
      NAME_FOR(STATE_EXIT1_IR);
      NAME_FOR(STATE_PAUSE_IR);
      NAME_FOR(STATE_EXIT2_IR);
      NAME_FOR(STATE_UPDATE_IR);
    default:
      return "STATE_WTF";
    }
  }

  void get_next_bytes_from_stream(uint8_t count) {
    while (count--) {
      *bp_++ = serial_comm_.GetNextByte();
    }
  }

  uint8_t get_next_byte() {
    return *bp_++;
  }

  uint16_t get_next_word() {
    return (uint16_t)get_next_byte() << 8 | get_next_byte();
  }

  uint32_t get_next_long() {
    return (uint32_t)get_next_byte() << 24 |
      (uint32_t)get_next_byte() << 16 |
      (uint32_t)get_next_byte() << 8 |
      get_next_byte();
  }

  void get_next_bytes(uint8_t* data, uint8_t count) {
    while (count--) {
      *data++ = get_next_byte();
    }
  }

  bool handle_XCOMPLETE() {
    serial_comm_.Important("XCOMPLETE");
    reached_xcomplete_ = true;
    return false;
  }

  bool handle_XTDOMASK() {
    get_next_bytes(tdomask_, sdrsize_bytes_);
    serial_comm_.DebugBytes("... tdomask now ", tdomask_, sdrsize_bytes_);
    return true;
  }

  bool handle_XSIR() {
    uint8_t bits = get_next_byte();
    get_next_bytes(tdi_, BYTES(bits));
    state_goto(STATE_SHIFT_IR);
    shift_td(tdi_, bits, true);
    state_goto(STATE_RTI);
    return true;
  }

  bool handle_XSDR() {
    get_next_bytes(tdi_, sdrsize_bytes_);
    serial_comm_.DebugBytes("... sending ", tdi_, sdrsize_bytes_);
    return sdr(true, true, true);
  }

  bool handle_XRUNTEST() {
    runtest_ = get_next_long();
    serial_comm_.Debug("... runtest now %ld", runtest_);
    return true;
  }

  bool handle_XREPEAT() {
    repeat_ = get_next_byte();
    serial_comm_.Debug("... repeat now %d", repeat_);
    return true;
  }

  bool handle_XSDRSIZE() {
    sdrsize_bits_ = get_next_long();
    sdrsize_bytes_ = BYTES(sdrsize_bits_);
    serial_comm_.Debug("... sdrsize now %d/%d", sdrsize_bits_, sdrsize_bytes_);
    return true;
  }

  bool handle_XSDRTDO() {
    get_next_bytes(tdi_, sdrsize_bytes_);
    get_next_bytes(tdo_expected_, sdrsize_bytes_);
    serial_comm_.DebugBytes("... sending   ", tdi_, sdrsize_bytes_);
    serial_comm_.DebugBytes("... expecting ", tdo_expected_, sdrsize_bytes_);
    return sdr(true, true, true);
  }

  bool is_tdo_as_expected() {
    serial_comm_.DebugBytes("... received  ", tdo_, sdrsize_bytes_);
    for (int i = 0; i < sdrsize_bytes_; ++i) {
      uint8_t expected = tdo_expected_[i] & tdomask_[i];
      uint8_t actual = tdo_[i] & tdomask_[i];
      if (expected != actual) {
        serial_comm_.Debug("... NO MATCH.");
        return false;
      }
    }
    serial_comm_.Debug("... match!");
    return true;
  }

  bool handle_XSTATE() {
    state_goto(get_next_byte());
    return true;
  }

  bool handle_XWAIT() {
    state_goto(get_next_byte());
    uint8_t end_state = get_next_byte();
    uint32_t wait_time_usec = get_next_long();
    wait_time(wait_time_usec);
    state_goto(end_state);
    return true;
  }

  void shift_td(uint8_t *data, uint16_t data_length_bits, bool is_end) {
    int byte_count = BYTE_COUNT(data_length_bits);

    for (int i = 0; i < byte_count; ++i) {
      uint8_t byte_out = data[byte_count - 1 - i];
      uint8_t tdo_byte = 0;
      for (int j = 0; j < 8 && data_length_bits-- > 0; ++j) {
        if (data_length_bits == 0 && is_end) {
          twiddler_.set_tms();
          state_ack(1);
        }

        if (byte_out & 1) {
          twiddler_.set_tdi();
        } else {
          twiddler_.clr_tdi();
        }
        byte_out >>= 1;
        bool tdo = twiddler_.pulse_clock_and_read_tdo();
        tdo_byte |= tdo << j;
      }
      tdo_[byte_count - 1 - i] = tdo_byte;
    }
  }

  bool sdr(bool should_begin, bool should_end, bool should_check) {
    int attempts_left = repeat_;
    bool matched = false;

    if (should_begin) {
      state_goto(STATE_SHIFT_DR);
    }

    while (!matched && attempts_left-- > 0) {
      shift_td(tdi_, sdrsize_bits_, should_end);
      if (should_check) {
        if (is_tdo_as_expected()) {
          matched = true;
        } else {
          state_goto(STATE_PAUSE_DR);
          state_goto(STATE_SHIFT_DR);
          state_goto(STATE_RTI);
          wait_time(runtest_);
          state_goto(STATE_SHIFT_DR);
        }
      }
    }
    if (should_check && !matched) {
      serial_comm_.Important("SDR check failed.");
      return false;
    }
    if (should_end) {
      state_goto(STATE_RTI);
    }
    wait_time(runtest_);
    return true;
  }

  void wait_time(uint32_t microseconds) {
    serial_comm_.Debug("Waiting %ld microseconds...", microseconds);
    uint32_t until = micros() + microseconds;
    while (microseconds--) {
      twiddler_.pulse_clock();
    }
    while (micros() < until) {
      twiddler_.pulse_clock();
    }
  }

  void set_state(int state) {
    current_state_ = state;
  }

  void state_ack(bool tms) {
    if (tms) {
      set_state((tms_transitions[current_state_] >> 4) & 0xf);
    } else {
      set_state(tms_transitions[current_state_] & 0xf);
    }
  }

  void state_step(bool tms) {
    if (tms) {
      twiddler_.set_tms();
    } else {
      twiddler_.clr_tms();
    }
    twiddler_.pulse_clock();
    state_ack(tms);
  }

  void state_goto(int state) {
    if (state == STATE_TLR) {
      for (int i = 0; i < 5; ++i) {
        state_step(true);
      }
    } else {
      while (current_state_ != state) {
        state_step((tms_map[current_state_] >> state) & 1);
      }
    }
  }

  SerialComm serial_comm_;
  BitTwiddler twiddler_;
  uint8_t current_state_;
  uint8_t sdrsize_bits_;
  uint8_t sdrsize_bytes_;
  uint8_t repeat_;
  uint32_t runtest_;
  bool reached_xcomplete_;

  enum {
    BUFFER_SIZE = 32
  };
  uint8_t instruction_buffer_[BUFFER_SIZE];
  uint8_t* bp_;
  uint8_t tdi_[BUFFER_SIZE];
  uint8_t tdo_[BUFFER_SIZE];
  uint8_t tdomask_[BUFFER_SIZE];
  uint8_t tdo_expected_[BUFFER_SIZE];
};

#endif  // INCLUDE_JTAG_WHISPERER_JTAG_WHISPERER_H
