^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package mavros_extras
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.11.2 (2015-04-26)
-------------------
* gcs bridge fix `#277 <https://github.com/vooon/mavros/issues/277>`_: add link diagnostics
* Contributors: Vladimir Ermakov

0.11.1 (2015-04-06)
-------------------
* mavftpfuse `#129 <https://github.com/vooon/mavros/issues/129>`_: done!
  Fix `#129 <https://github.com/vooon/mavros/issues/129>`_.
* mavftpfuse `#129 <https://github.com/vooon/mavros/issues/129>`_: cache file attrs
* mavftpfuse `#129 <https://github.com/vooon/mavros/issues/129>`_: initial import
* Contributors: Vladimir Ermakov

0.11.0 (2015-03-24)
-------------------
* extras: vision_pose `#247 <https://github.com/vooon/mavros/issues/247>`_: rename topic
* extras: launch `#257 <https://github.com/vooon/mavros/issues/257>`_: use white list for px4flow.
  Also updates config `#211 <https://github.com/vooon/mavros/issues/211>`_.
* uncrustify and fix `#207 <https://github.com/vooon/mavros/issues/207>`_
* uncrustify extras
* package: update lic
* license `#242 <https://github.com/vooon/mavros/issues/242>`_: update mavros_extras headers
* plugin api `#241 <https://github.com/vooon/mavros/issues/241>`_: move diag updater to UAS.
* plugin api `#241 <https://github.com/vooon/mavros/issues/241>`_: remove global private node handle.
  Now all plugins should define their local node handle (see dummy.cpp).
  Also partially does `#233 <https://github.com/vooon/mavros/issues/233>`_ (unmerge setpoint topic namespace).
* plugin api `#241 <https://github.com/vooon/mavros/issues/241>`_: remove `get_name()`
* Add BSD license option `#220 <https://github.com/vooon/mavros/issues/220>`_
* uncrustify: mocap plugin
* Switched from mavlink VICON_POSITION_ESTIMATE to ATT_POS_MOCAP.
* Contributors: Tony Baltovski, Vladimir Ermakov

0.10.2 (2015-02-25)
-------------------
* launch: Fix vim modelines `#213 <https://github.com/vooon/mavros/issues/213>`_
* Contributors: Vladimir Ermakov

0.10.1 (2015-02-02)
-------------------
* Fix @mhkabir name in contributors.
* Updated mavros_extra README to explain the vision_estimate plugin should be used for the mocap data currently.
* Update px4flow.cpp
* plguin: px4flow: Remove all ref to old message
* Merge remote-tracking branch 'upstream/master' into optflow_rad
  Conflicts:
  mavros_extras/CMakeLists.txt
* Update
* Clean up
* New interface commit
* Add new interface. Raw message only for now. Removed the tx functionality as it doesn't make much sense.
* Contributors: Mohammed Kabir, Tony Baltovski, Vladimir Ermakov

0.10.0 (2015-01-24)
-------------------
* mocap_pose_estimate: Switched from pose to poseStamped.
* Contributors: Tony Baltovski

0.9.4 (2015-01-06)
------------------

0.9.3 (2014-12-30)
------------------
* Initiliser fix
* plugin: visualisation - Fixes CI build
* plugin: visualisation
* plugin: visualization minor patch
* plugin: visualization finshed
* Contributors: Mohammed Kabir

0.9.2 (2014-11-04)
------------------

0.9.1 (2014-11-03)
------------------

0.9.0 (2014-11-03)
------------------

0.8.2 (2014-11-03)
------------------
* REP140: update package.xml format.
  Hydro don't accept this format correctly,
  but after split i can update.
* Contributors: Vladimir Ermakov

0.8.1 (2014-11-02)
------------------
* mavconn `#161 <https://github.com/vooon/mavros/issues/161>`_: Fix headers used in mavros. Add readme.
* Update repo links.
  Package moved to mavlink organization.
* Contributors: Vladimir Ermakov

0.8.0 (2014-09-22)
------------------
* Revert "Update package.xml format to REP140 (2)."
  This reverts commit 81286eb84090a95759591cfab89dd9718ff35b7e.
  ROS Hydro don't fully support REP140: rospack can't find plugin
  descriptions.
  Fix `#151 <https://github.com/vooon/mavros/issues/151>`_.
* Added arming/disarming for att mode.
* Added arming and disarming via mavteleop.
* extras: mocap: Fix param/topic namespace.
  Fix `#150 <https://github.com/vooon/mavros/issues/150>`_.
* extras: launch: Use includes.
  Fix `#144 <https://github.com/vooon/mavros/issues/144>`_.
* Update package.xml format to REP140 (2).
  Fix `#104 <https://github.com/vooon/mavros/issues/104>`_.
* extras: launch: Fix typos.
* extras: launch: Add teleop launch script.
* extras: mavteleop: Dirty implementation of position control mode.
  Issue `#133 <https://github.com/vooon/mavros/issues/133>`_.
* extras: mavteleop: Implement velocity setpoint control.
  Issue `#133 <https://github.com/vooon/mavros/issues/133>`_.
* extras: mavteleop: Implement attitude control mode.
  Issue `#133 <https://github.com/vooon/mavros/issues/133>`_.
* extras: Use cmake modules.
  Issue `#139 <https://github.com/vooon/mavros/issues/139>`_.
* Update doxygen documentation.
  Add split lines in UAS, and make UAS.connection atomic.
  Add rosdoc configuration for mavros_extras.
* scripts: mavsetp: corrected msg API; mavteleop: added prefix to rc override
* scripts: Initial import mavteleop
  Now it's just proof of concept.
  Implemented only RC override of RPYT channels.
  Issue `#133 <https://github.com/vooon/mavros/issues/133>`_.
* node: Catch URL open exception.
  Also update connection pointer type.
* Contributors: Nuno Marques, Tony Baltovski, Vladimir Ermakov

0.7.1 (2014-08-25)
------------------
* plugins: Change UAS FCU link name.
  Reduce smart pointer count, that hold fcu link object.
* Plugins: finish moving plugins
* Closes `#122 <https://github.com/vooon/mavros/issues/122>`_, closes `#123 <https://github.com/vooon/mavros/issues/123>`_; plugins: move mocap & vision plugins to extras, change vision plugins name
* launch: Add example launch for `#103 <https://github.com/vooon/mavros/issues/103>`_.
* extras: image_pub: Update plugin API.
* extras: px4flow: Update plugin API.
* plugins: disable most of plugins
* extras: init ctor
* extras: Fix package URLs
* test: temporary travis hack (manually download latest mavlink deb)
* Update readme
* Contributors: Nuno Marques, Vladimir Ermakov

0.7.0 (2014-08-12)
------------------
* move exras to subdirectory, `#101 <https://github.com/vooon/mavros/issues/101>`_
* Contributors: Vladimir Ermakov, Mohammed Kabir
