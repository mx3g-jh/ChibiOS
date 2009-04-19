/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <ch.h>

#include "test.h"

#if CH_USE_MESSAGES

static char *msg1_gettest(void) {

  return "Messages, loop";
}

static msg_t thread(void *p) {

  chMsgSend(p, 'A');
  chMsgSend(p, 'B');
  chMsgSend(p, 'C');
  chMsgSend(p, 'D');
  return 0;
}

static void msg1_execute(void) {
  msg_t msg;

  /*
   * Testing the whole messages loop.
   */
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() + 1,
                                 thread, chThdSelf());
  chMsgRelease(msg = chMsgWait());
  test_emit_token(msg);
  chMsgRelease(msg = chMsgWait());
  test_emit_token(msg);
  chMsgRelease(msg = chMsgWait());
  test_emit_token(msg);
  test_assert_sequence("ABC");

  /*
   * Testing message fetch using chMsgGet().
   * Note, the following is valid because the sender has higher priority than
   * the receiver.
   */
  msg = chMsgGet();
  test_assert(msg != 0, "#1");
  chMsgRelease(0);
  test_assert(msg == 'D', "#2");

  /*
   * Must not have pending messages.
   */
  msg = chMsgGet();
  test_assert(msg == 0, "#3");
}

const struct testcase testmsg1 = {
  msg1_gettest,
  NULL,
  NULL,
  msg1_execute
};

#endif /* CH_USE_MESSAGES */

/*
 * Test sequence for messages pattern.
 */
const struct testcase * const patternmsg[] = {
#if CH_USE_MESSAGES
  &testmsg1,
#endif
  NULL
};
