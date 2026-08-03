.. _i3c-controller-test:

I3C Controller Test (Hot Join / IBI)
####################################

Overview
********

Controller-side companion to the ``i3c_target_test`` target sample. It drives the
controller-mode I3C instance selected by the ``i3c-controller-0`` devicetree
alias and exercises Hot Join and IBI reception:

* Hot Join is auto-accepted by the RTK core (ACK + ENTDAA). A hot-joined target
  declared under the controller node (matched by PID) gets a dynamic address
  and appears in the attached device list.
* IBI reception is enabled per target via ``i3c_ibi_enable()`` (ENEC INTR) after
  registering a callback; received IBIs are logged.

Build
*****

.. code-block:: console

   west build -c -p always -b rts5918_evb -s zephyr/samples/i3c_controller_test

Set ``i3c-controller-0`` in ``boards/rts5918_evb.overlay`` to your
controller-mode instance, and declare the expected target(s) as child nodes of
that controller node (by PID) so Hot Join / DAA can bind them to a descriptor.

Shell commands
**************

* ``i3c_controller ibi-enable``  - arm auto-IBI: (hot-)joined targets get ENEC'd automatically
* ``i3c_controller ibi-disable`` - disarm auto-IBI and DISEC all targets
* ``i3c_controller daa``         - manually run dynamic address assignment
* ``i3c_controller list``        - list attached I3C devices

Typical flow
************

The upper layer only arms once; Hot Join then drives the rest automatically:

1. Boot, then run ``i3c_controller ibi-enable`` once to arm.
2. Hot-join / power the target. The RTK core auto-accepts the Hot Join and runs
   DAA; a background poll then auto-ENECs the newly addressed, IBI-capable
   target -- no further commands needed.
3. When the target raises an IBI it is logged as ``IBI from 0x.. : ...``.
4. ``i3c_controller list`` shows attached devices; ``i3c_controller ibi-disable`` disarms.
