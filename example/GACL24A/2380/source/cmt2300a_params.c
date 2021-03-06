#include "cmt2300a_params.h"

word SysTime;			//TODO: these variable to be rm from this file
//FlagSTR  _System_F;
//uint8_t Prodution_ID[3];

uint8_t TX_Bufs[64];

word ResetTime;
//FlagSTR  		_LedTime;

//;---------------------------------------
//;  CMT2300A Configuration File
//;  Generated by CMOSTEK RFPDK 1.51
//;  2022.03.23 11:08
//;---------------------------------------
//; Mode                      = Advanced
//; Part Number               = CMT2300A
//; Frequency                 = 868.000 MHz
//; Xtal Frequency            = 26.0000 MHz
//; Demodulation              = GFSK
//; AGC                       = On
//; Data Rate                 = 50.0 kbps
//; Deviation                 = 50.0 kHz
//; Tx Xtal Tol.              = 20 ppm
//; Rx Xtal Tol.              = 20 ppm
//; TRx Matching Network Type = 20 dBm
//; Tx Power                  = +20 dBm
//; Gaussian BT               = 0.5
//; Bandwidth                 = Auto-Select kHz
//; CDR Type                  = Counting
//; CDR DR Range              = NA
//; AFC                       = On
//; AFC Method                = Auto-Select
//; Data Representation       = 0:F-low 1:F-high
//; Rx Duty-Cycle             = Off
//; Tx Duty-Cycle             = Off
//; Sleep Timer               = Off
//; Sleep Time                = NA
//; Rx Timer                  = Off
//; Rx Time T1                = NA
//; Rx Time T2                = NA
//; Rx Exit State             = STBY
//; Tx Exit State             = STBY
//; SLP Mode                  = Disable
//; RSSI Valid Source         = PJD
//; PJD Window                = 8 Jumps
//; LFOSC Calibration         = On
//; Xtal Stable Time          = 155 us
//; RSSI Compare TH           = NA
//; Data Mode                 = Packet
//; Whitening                 = Disable
//; Whiten Type               = NA
//; Whiten Seed Type          = NA
//; Whiten Seed               = NA
//; Manchester                = Disable
//; Manchester Type           = NA
//; FEC                       = Disable
//; FEC Type                  = NA
//; Tx Prefix Type            = 0
//; Tx Packet Number          = 1
//; Tx Packet Gap             = 32
//; Packet Type               = Variable Length
//; Node-Length Position      = First Length, then Node
//; Payload Bit Order         = Start from msb
//; Preamble Rx Size          = 2
//; Preamble Tx Size          = 8
//; Preamble Value            = 85
//; Preamble Unit             = 8-bit
//; Sync Size                 = 4-uint8_t
//; Sync Value                = 2466992606
//; Sync Tolerance            = None
//; Sync Manchester           = Disable
//; Node ID Size              = NA
//; Node ID Value             = NA
//; Node ID Mode              = None
//; Node ID Err Mask          = Disable
//; Node ID Free              = Disable
//; Payload Length            = 32
//; CRC Options               = IBM-16
//; CRC Seed                  = 65535 crc_seed
//; CRC Range                 = Entire Payload
//; CRC Swap                  = Start from MSB
//; CRC Bit Invert            = Normal
//; CRC Bit Order             = Start from bit 15
//; Dout Mute                 = Off
//; Dout Adjust Mode          = Disable
//; Dout Adjust Percentage    = NA
//; Collision Detect          = Off
//; Collision Detect Offset   = NA
//; RSSI Detect Mode          = Always
//; RSSI Filter Setting       = No Filtering
//; RF Performance            = High
//; LBD Threshold             = 2.4 V
//; RSSI Offset               = 0
//; RSSI Offset Sign          = 0		

uint8_t g_cmt2300aCmtBank[12] = {
0x00,
0x66,
0xEC,
0x1D,
0x70,
0x80,
0x14,
0x08,
0x11,
0x02,
0x02,
0x00,
};

//[System Bank]
uint8_t g_cmt2300aSystemBank[12] = {
0xAE,
0xE0,
0x35,
0x00,
0x00,
0xF4,
0x10,
0xE2,
0x42,
0x20,
0x00,
0x81,
};

//[Frequency Bank]
uint8_t g_cmt2300aFrequencyBank[8] = {
0x42,
0xCF,
0xA7,
0x8C,
0x42,
0xC4,
0x4E,
0x1C,
};

//[Data Rate Bank]
uint8_t g_cmt2300aDataRateBank[24] = {
0x1F,
0xF8,
0x21,
0x10,
0x72,
0x12,
0x1F,
0x0A,
0x9F,
0x6C,
0x29,
0x29,
0xC0,
0x08,
0x02,
0x53,
0x18,
0x00,
0xB4,
0x00,
0x00,
0x01,
0x00,
0x00,
};

//[Baseband Bank]
uint8_t g_cmt2300aBasebandBank[29] = {
0x12,
0x08,
0x00,
0x55,
0x06,
0x00,
0x00,
0x00,
0x00,
0xDE,
0x51,
0x0B,
0x93,
0x05,//0x05,	//05 for variable lenth, 00 for fix lenth
0x1F,
0x00,
0x00,
0x00,
0x00,
0x00,
0x03,
0xFF,
0xFF,
0x60,
0xFF,
0x00,
0x00,
0x1F,
0x10,
};

//[TX Bank]
uint8_t g_cmt2300aTxBank[11] = {
0x70,
0xC0,
0x0F,
0x00,
0x02,
0x30,
0x00,
0x8A,
0x18,
0x3F,
0x7F,
};


