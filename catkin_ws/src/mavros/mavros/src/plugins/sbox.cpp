/**
 * @brief SherpaBox status plugin
 * @file sbox.cpp
 * @author SherpaBox TEAM
 *
 * @addtogroup plugin
 * @{
 */

#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>

#include <mavros/RadioStatus.h>

#include <sbox_msgs/Sbox_msg_commands.h>
#include <sbox_msgs/Sbox_msg_status.h>

namespace mavplugin {
/**
 * @brief SherpaBox plugin.
 */
class SboxPlugin : public MavRosPlugin {
public:
	SboxPlugin() :
		nh("~"),
		has_radio_status(false),
		diag_added(false),
		low_rssi(0)
	{ }
	void initialize(UAS &uas_)
	{
		uas = &uas_;

		nh.param("sbox/low_rssi", low_rssi, 40);

/* --- SUBSCRIPTIONS --- */
		command_sub = nh.subscribe("Sbox_msg_commands", 10, &SboxPlugin::command_cb, this);
/* --- PUBLISHERS --- */
		status_pub = nh.advertise<mavros::RadioStatus>("radio_status", 10);

		uas->sig_connection_changed.connect(boost::bind(&SboxPlugin::connection_cb, this, _1));
	}

	const message_map get_rx_handlers() {
		return {
			       MESSAGE_HANDLER(MAVLINK_MSG_ID_RADIO_STATUS, &SboxPlugin::handle_radio_status),
//#ifdef MAVLINK_MSG_ID_RADIO
//			       MESSAGE_HANDLER(MAVLINK_MSG_ID_RADIO, &SboxPlugin::handle_radio),
//#endif
		};
	}

private:
	ros::NodeHandle nh;
	UAS *uas;

	bool has_radio_status;
	bool diag_added;
	int low_rssi;

	ros::Subscriber command_sub;
	ros::Publisher status_pub;

	std::recursive_mutex diag_mutex;
	mavros::RadioStatus::Ptr last_status;

	/* -*- message handlers -*- */

	void handle_radio_status(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {
		mavlink_radio_status_t rst;
		mavlink_msg_radio_status_decode(msg, &rst);
		has_radio_status = true;
		handle_message(rst, sysid, compid);
	}

	template<typename msgT>
	void handle_message(msgT &rst, uint8_t sysid, uint8_t compid) {
		if (sysid != '3' || compid != 'D')
			ROS_WARN_THROTTLE_NAMED(30, "radio", "RADIO_STATUS not from 3DR modem?");

		auto msg = boost::make_shared<mavros::RadioStatus>();

		msg->header.stamp = ros::Time::now();

#define RST_COPY(field)	msg->field = rst.field
		RST_COPY(rssi);
		RST_COPY(remrssi);
		RST_COPY(txbuf);
		RST_COPY(noise);
		RST_COPY(remnoise);
		RST_COPY(rxerrors);
		RST_COPY(fixed);
#undef RST_COPY

		// valid for 3DR modem
		msg->rssi_dbm = (rst.rssi / 1.9) - 127;
		msg->remrssi_dbm = (rst.remrssi / 1.9) - 127;

		// add diag at first event
		if (!diag_added) {
			UAS_DIAG(uas).add("SherpaBox", this, &SboxPlugin::diag_run);
			diag_added = true;
		}

		// store last status for diag
		{
			lock_guard lock(diag_mutex);
			last_status = msg;
		}

		status_pub.publish(msg);
	}


	void diag_run(diagnostic_updater::DiagnosticStatusWrapper &stat) {
		lock_guard lock(diag_mutex);

		if (!last_status) {
			stat.summary(2, "No data");
			return;
		}
		else if (last_status->rssi < low_rssi)
			stat.summary(1, "Low RSSI");
		else if (last_status->remrssi < low_rssi)
			stat.summary(1, "Low remote RSSI");
		else
			stat.summary(0, "Normal");

		stat.addf("RSSI", "%u", last_status->rssi);
		stat.addf("RSSI (dBm)", "%.1f", last_status->rssi_dbm);
		stat.addf("Remote RSSI", "%u", last_status->remrssi);
		stat.addf("Remote RSSI (dBm)", "%.1f", last_status->remrssi_dbm);
		stat.addf("Tx buffer (%)", "%u", last_status->txbuf);
		stat.addf("Noice level", "%u", last_status->noise);
		stat.addf("Remote noice level", "%u", last_status->remnoise);
		stat.addf("Rx errors", "%u", last_status->rxerrors);
		stat.addf("Fixed", "%u", last_status->fixed);
	}

	void connection_cb(bool connected) {
		UAS_DIAG(uas).removeByName("SherpaBox");
		diag_added = false;
	}

	/*
	 * Arming/disarming the UAV
	 */
	void command_cb(const sbox_msgs::Sbox_msg_commands::ConstPtr msg){
		mavlink_message_t msg_mav;
			
		uint8_t noise = msg->executeId; //Tipo comando
		uint8_t rssi = msg->parameters; //Parametro
		uint16_t rxerrors = 0;
		uint16_t fixed = 0;
		uint8_t remrssi = 0;
		uint8_t txbuf = 0;
		uint8_t remnoise = 0;

//static inline uint16_t mavlink_msg_radio_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, uint8_t rssi,uint8_t remrssi,uint8_t txbuf,uint8_t noise,uint8_t remnoise,uint16_t rxerrors,uint16_t fixed)

mavlink_radio_t rt;
rt.noise=msg->executeId;
rt.rssi = msg->parameters;
rt.rxerrors = 0;
rt.fixed = 0;
rt.remrssi = 0;
rt.txbuf = 0;
rt.remnoise = 0;

//static inline uint16_t mavlink_msg_radio_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_radio_t* radio)

		mavlink_msg_radio_encode(uas->get_tgt_system(), uas->get_tgt_component(), &msg_mav, &rt);
/*
			mavlink_msg_radio_pack_chan(UAS_PACK_CHAN(uas), &msg_mav,
								uas->get_tgt_system(),
								uas->get_tgt_component(),
								
								rssi, remrssi,
								txbuf, noise,
								remnoise, rxerrors,
								fixed);
*/

			UAS_FCU(uas)->send_message(&msg_mav);
			ROS_INFO("Send SBOX Command!");
	}

};
};	// namespace mavplugin

PLUGINLIB_EXPORT_CLASS(mavplugin::SboxPlugin, mavplugin::MavRosPlugin)

