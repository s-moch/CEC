#ifndef CECWIRE_H__
#define CECWIRE_H__

#include "Common.h"

class CEC_Electrical
{
public:
	typedef enum {
		CDT_TV,
		CDT_RECORDING_DEVICE,
		CDT_PLAYBACK_DEVICE,
		CDT_TUNER,
		CDT_AUDIO_SYSTEM,
	} CEC_DEVICE_TYPE;

public:
	CEC_Electrical();
	void Initialize(CEC_DEVICE_TYPE type);
	bool TransmitFrame(int targetAddress, unsigned char* buffer, int count);
	void Run();

	int Promiscuous;
	int MonitorMode;

protected:
	virtual bool LineState() = 0;
	virtual void SetLineState(bool) = 0;
	virtual void OnTransmitComplete(unsigned char* buffer, int count, bool ack) = 0;
	virtual void OnReceiveComplete(unsigned char* buffer, int count, bool ack) = 0;
	virtual void OnReady(int logicalAddress) = 0;

private:
	bool Transmit(int sourceAddress, int targetAddress, unsigned char* buffer, unsigned int count);

private:
	// CEC locical address handling
	typedef enum {
		CLA_TV = 0,
		CLA_RECORDING_DEVICE_1,
		CLA_RECORDING_DEVICE_2,
		CLA_TUNER_1,
		CLA_PLAYBACK_DEVICE_1,
		CLA_AUDIO_SYSTEM,
		CLA_TUNER_2,
		CLA_TUNER_3,
		CLA_PLAYBACK_DEVICE_2,
		CLA_RECORDING_DEVICE_3,
		CLA_TUNER_4,
		CLA_PLAYBACK_DEVICE_3,
		CLA_RESERVED_1,
		CLA_RESERVED_2,
		CLA_FREE_USE,
		CLA_UNREGISTERED,
	} CEC_LOGICAL_ADDRESS;

	int _logicalAddress;
	const char *_validLogicalAddresses;

	// Receive buffer
	unsigned char _receiveBuffer[16];
	unsigned int _receiveBufferBits;

	// Transmit buffer
	unsigned char _transmitBuffer[16];
	unsigned int _transmitBufferBytes;
	unsigned int _transmitBufferBitIdx;

	// State machine
	typedef enum {
		CEC_IDLE,

		CEC_RCV_STARTBIT1,
		CEC_RCV_STARTBIT2,
		CEC_RCV_DATABIT1,
		CEC_RCV_DATABIT2,
		CEC_RCV_EOM1,
		CEC_RCV_EOM2,
		CEC_RCV_ACK_SENT,
		CEC_RCV_ACK1,
		CEC_RCV_ACK2,
		CEC_RCV_LINEERROR,

		CEC_XMIT_WAIT,
		CEC_XMIT_STARTBIT1,
		CEC_XMIT_STARTBIT2,
		CEC_XMIT_DATABIT1,
		CEC_XMIT_DATABIT2,
		CEC_XMIT_EOM1,
		CEC_XMIT_EOM2,
		CEC_XMIT_ACK1,
		CEC_XMIT_ACK_TEST,
		CEC_XMIT_ACK_WAIT,
		CEC_XMIT_ACK2,
	} CEC_STATE;

	enum {
		STARTBIT_TIME_LOW   = 3700, // 3.7ms
		STARTBIT_TIME       = 4500, // 4.5ms
		BIT_TIME_LOW_0      = 1500, // 1.5ms
		BIT_TIME_LOW_1      =  600, // 0.6ms
		BIT_TIME_SAMPLE     = 1050, // 1.05ms
		BIT_TIME            = 2400, // 2.4ms
		BIT_TIME_ERR        = 3600, // 3.6ms
		BIT_TIME_LOW_MARGIN =  300, // 0.2ms  plus some additional margin since we poll the bitline
		BIT_TIME_MARGIN     =  450, // 0.35ms plus some additional margin since we poll the bitline
	};

	bool _lastLineState;
	unsigned long _bitStartTime;
	unsigned int _waitTime;

	int _xmitretry;
	enum {
		CEC_MAX_RETRANSMIT = 5,
	};

	bool _eom;
	bool _ack;
	bool _follower;
	bool _broadcast;
	bool _amLastTransmittor;

	CEC_STATE _state;
};

#endif // CECWIRE_H__
