#pragma once

#include "Arduino.h"
#include "crc8.h"
#include "crsf_protocol.h"

enum eFailsafeAction { fsaNoPulses, fsaHold };

class AlfredoCRSF
{
public:
    // Packet timeout where buffer is flushed if no data is received in this time
    static const unsigned int CRSF_PACKET_TIMEOUT_MS = 100;
    static const unsigned int CRSF_FAILSAFE_STAGE1_MS = 300;

    AlfredoCRSF();
    void begin(Stream& port);
    void update();
    void write(uint8_t b);
    void write(const uint8_t *buf, size_t len);
    void queuePacket(uint8_t addr, uint8_t type, const void *payload, uint8_t len);
    void writePacket(uint8_t addr, uint8_t type, const void *payload, uint8_t len);

    // Return current channel value (1-based) in us
    int getChannel(unsigned int ch) const { return _channels[ch - 1]; }
    const crsf_channels_t *getChannelsPacked() const { return &_channelsPacked;}
    const crsfLinkStatistics_t *getLinkStatistics() const { return &_linkStatistics; }
    const crsf_sensor_gps_t *getGpsSensor() const { return &_gpsSensor; }
    const crsf_sensor_vario_t *getVarioSensor() const { return &_varioSensor; }
    const crsf_sensor_baro_altitude_t *getBaroAltitudeSensor() const { return &_baroAltitudeSensor; }
    const crsf_sensor_attitude_t *getAttitudeSensor() const { return &_attitudeSensor; }
    bool isLinkUp() const { return _linkIsUp; }

private:
    Stream* _port;
    uint8_t _rxBuf[CRSF_MAX_PACKET_LEN+3];
    uint8_t _rxBufPos;
    Crc8 _crc;
    crsf_channels_t _channelsPacked;
    crsfLinkStatistics_t _linkStatistics;
    crsf_sensor_gps_t _gpsSensor;
    crsf_sensor_vario_t _varioSensor;
    crsf_sensor_baro_altitude_t _baroAltitudeSensor;
    crsf_sensor_attitude_t _attitudeSensor;
    uint32_t _baud;
    uint32_t _lastReceive;
    uint32_t _lastChannelsPacket;
    bool _linkIsUp;
    int _channels[CRSF_NUM_CHANNELS];

    void handleSerialIn();
    void handleByteReceived();
    void shiftRxBuffer(uint8_t cnt);
    void processPacketIn(uint8_t len);
    void checkPacketTimeout();
    void checkLinkDown();

    // Packet RX Handlers
    void packetChannelsPacked(const crsf_header_t *p);
    void packetLinkStatistics(const crsf_header_t *p);
    void packetGps(const crsf_header_t *p);
    void packetVario(const crsf_header_t *p);
    void packetBaroAltitude(const crsf_header_t *p);
    void packetAttitude(const crsf_header_t *p);
};
