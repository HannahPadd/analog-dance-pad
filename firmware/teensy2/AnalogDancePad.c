/*
  Based on LUFA Library example code (www.lufa-lib.org):

  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  See other copyrights in LICENSE file on repository root.

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include <stdlib.h>
#include <string.h>

#include "Config/DancePadConfig.h"
#include "AnalogDancePad.h"
#include "Communication.h"
#include "Descriptors.h"
#include "Pad.h"
#include "Reset.h"
#include "ConfigStore.h"
#include "Lights.h"

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[GENERIC_EPSIZE];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Generic_HID_Interface =
    {
        .Config =
            {
                .InterfaceNumber              = INTERFACE_ID_GenericHID,
                .ReportINEndpoint             =
                    {
                        .Address              = GENERIC_IN_EPADDR,
                        .Size                 = GENERIC_EPSIZE,
                        .Banks                = 1,
                    },
                .PrevReportINBuffer           = PrevHIDReportBuffer,
                .PrevReportINBufferSize       = sizeof(PrevHIDReportBuffer),
            },
    };

static Configuration configuration;

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
    SetupHardware();
    GlobalInterruptEnable();
    ConfigStore_LoadConfiguration(&configuration);
    Pad_Initialize(&configuration.padConfiguration);
    Lights_UpdateConfiguration(&configuration.lightConfiguration);
	Lights_Update();

    for (;;)
    {
        HID_Device_USBTask(&Generic_HID_Interface);
        USB_USBTask();
    }
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
    /* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
    XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
    XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

    /* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
    XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
    XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

    PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

    /* Hardware Initialization */
    USB_Init();
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    HID_Device_ConfigureEndpoints(&Generic_HID_Interface);
    USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
    HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
    if (*ReportID == 0) {
        // no report id requested - write button and sensor data
        Communication_WriteInputHIDReport(ReportData);
        *ReportID = INPUT_REPORT_ID;
        *ReportSize = sizeof (InputHIDReport);
    } else if (*ReportID == PAD_CONFIGURATION_REPORT_ID) {
        PadConfigurationFeatureHIDReport* configurationHidReport = ReportData;
        configurationHidReport->configuration = PAD_CONF;
        *ReportSize = sizeof (PadConfigurationFeatureHIDReport);
    } else if (*ReportID == NAME_REPORT_ID) {
        NameFeatureHIDReport* nameHidReport = ReportData;
        memcpy(&nameHidReport->nameAndSize, &configuration.nameAndSize, sizeof (nameHidReport->nameAndSize));
        *ReportSize = sizeof (NameFeatureHIDReport);
    } else if (*ReportID == LIGHTS_REPORT_ID) {
        LightsFeatureHIDReport* lightsHidReport = ReportData;
        memcpy(&lightsHidReport->lightConfiguration, &configuration.lightConfiguration, sizeof (lightsHidReport->lightConfiguration));
        *ReportSize = sizeof (LightsFeatureHIDReport);
    }
    
    return true;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
    if (ReportID == PAD_CONFIGURATION_REPORT_ID && ReportSize == sizeof (PadConfigurationFeatureHIDReport)) {
        const PadConfigurationFeatureHIDReport* configurationHidReport = ReportData;
        memcpy(&configuration.padConfiguration, &configurationHidReport->configuration, sizeof (configuration.padConfiguration));
        Pad_UpdateConfiguration(&configurationHidReport->configuration);
    } else if (ReportID == RESET_REPORT_ID) {
        Reset_JumpToBootloader();
    } else if (ReportID == SAVE_CONFIGURATION_REPORT_ID) {
        ConfigStore_StoreConfiguration(&configuration);
    } else if (ReportID == FACTORY_RESET_REPORT_ID) {
        ConfigStore_FactoryDefaults(&configuration);
        ConfigStore_StoreConfiguration(&configuration);
    } else if (ReportID == NAME_REPORT_ID && ReportSize == sizeof (NameFeatureHIDReport)) {
        const NameFeatureHIDReport* nameHidReport = ReportData;
        memcpy(&configuration.nameAndSize, &nameHidReport->nameAndSize, sizeof (configuration.nameAndSize));
    } else if (ReportID == LIGHTS_REPORT_ID && ReportSize == sizeof (LightsFeatureHIDReport)) {
        const LightsFeatureHIDReport* lightsHidReport = ReportData;
        memcpy(&configuration.lightConfiguration, &lightsHidReport->lightConfiguration, sizeof (configuration.lightConfiguration));
    }
}
