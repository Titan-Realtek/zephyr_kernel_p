.. _i3c-target-test-sample:

I3C Target Test
###############

Overview
********

Registers an echo target on every I3C target exposed through the
``i3c-target-<n>`` devicetree aliases (``i3c-target-0`` .. ``i3c-target-3``).
Whatever an I3C controller writes to the target is stored and returned on
the next read, providing a simple controller<->target loopback for validating
private/legacy transfers. The alias to node mapping is supplied by a board
overlay (see ``boards/``), keeping the application source board-agnostic.

The echo path uses the Zephyr I3C target byte callbacks: received bytes are
accumulated in ``write_received_cb``; on STOP the length is latched and a worker
thread preloads the same bytes into the TX FIFO via :c:func:`i3c_target_tx_write`
(the preload cannot run in the ``stop_cb`` ISR context). Echo is clean up to the
target FIFO depth (64 bytes).

Building and Running
********************

.. code-block:: console

   west build -c -p always -b board_name -s zephyr/samples/i3c_target_test

Then drive the paired controller from the ``i3c`` shell (write a few bytes,
read them back) to observe the echo, or use the ``i3c_target`` shell commands
(``status`` / ``hj`` / ``ibi`` / ``ibi-none`` / ``ibi-big``) on the target side.
