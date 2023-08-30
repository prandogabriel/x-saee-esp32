/*
 * AWS IoT Device SDK for Embedded C 202103.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file iot_demo_main.c
 *
 * @brief Demo for showing how to use the Device Shadow library's API. This version
 * of Device Shadow API provide macros and helper functions for assembling MQTT topics
 * strings, and for determining whether an incoming MQTT message is related to a
 * device shadow. The shadow can be either the classic shadow or a named shadow. Change
 * #iot_NAME to select the shadow. The Device Shadow library does not depend on a MQTT library,
 * therefore the code for MQTT connections are placed in another file (iot_demo_helpers.c)
 * to make it easy to read the code using Device Shadow library.
 *
 * This example assumes there is a powerOn state in the device shadow. It does the
 * following operations:
 * 1. Establish a MQTT connection by using the helper functions in iot_demo_helpers.c.
 * 2. Assemble strings for the MQTT topics of device shadow, by using macros defined by the Device Shadow library.
 * 3. Subscribe to those MQTT topics by using helper functions in iot_demo_helpers.c.
 * 4. Publish a desired state of powerOn by using helper functions in iot_demo_helpers.c.  That will cause
 * a delta message to be sent to device.
 * 5. Handle incoming MQTT messages in eventCallback, determine whether the message is related to the device
 * shadow by using a function defined by the Device Shadow library (iot_MatchTopicString). If the message is a
 * device shadow delta message, set a flag for the main function to know, then the main function will publish
 * a second message to update the reported state of powerOn.
 * 6. Handle incoming message again in eventCallback. If the message is from update/accepted, verify that it
 * has the same clientToken as previously published in the update message. That will mark the end of the demo.
 */

/* Standard includes. */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* POSIX includes. */
#include <unistd.h>
#include <inttypes.h>

/* shadow demo helpers header. */
#include "iot_helpers.h"

/* shadow light header. */
#include "light.h"

/* Shadow config include. */
#include "config.h"

/* SHADOW API header. */
#include "shadow.h"

/* JSON API header. */
#include "core_json.h"

/* Clock for timer. */
#include "clock.h"

/**
 * @brief This example uses the MQTT library of the AWS IoT Device SDK for
 * Embedded C. This is the prototype of the callback function defined by
 * that library. It will be invoked whenever the MQTT library receives an
 * incoming message.
 *
 * @param[in] pMqttContext MQTT context pointer.
 * @param[in] pPacketInfo Packet Info pointer for the incoming packet.
 * @param[in] pDeserializedInfo Deserialized information from the incoming packet.
 */
static void eventCallback(MQTTContext_t *pMqttContext,
                          MQTTPacketInfo_t *pPacketInfo,
                          MQTTDeserializedInfo_t *pDeserializedInfo);

/*-----------------------------------------------------------*/

/* This is the callback function invoked by the MQTT stack when it receives
 * incoming messages. This function demonstrates how to use the iot_MatchTopicString
 * function to determine whether the incoming message is a device shadow message
 * or not. If it is, it handles the message depending on the message type.
 */
static void eventCallback(MQTTContext_t *pMqttContext,
                          MQTTPacketInfo_t *pPacketInfo,
                          MQTTDeserializedInfo_t *pDeserializedInfo)
{
    (void)pMqttContext;

    assert(pDeserializedInfo != NULL);
    assert(pMqttContext != NULL);
    assert(pPacketInfo != NULL);

    uint16_t packetIdentifier = pDeserializedInfo->packetIdentifier;

    /* Handle incoming publish. The lower 4 bits of the publish packet
     * type is used for the dup, QoS, and retain flags. Hence masking
     * out the lower bits to check if the packet is publish. */
    if ((pPacketInfo->type & 0xF0U) == MQTT_PACKET_TYPE_PUBLISH)
    {
        assert(pDeserializedInfo->pPublishInfo != NULL);
        LogInfo(("pPublishInfo->pTopicName:%s.", pDeserializedInfo->pPublishInfo->pTopicName));

        Toggle_light();

    }
    else
    {
        HandleOtherIncomingPacket(pPacketInfo, packetIdentifier);
    }
}

int aws_iot_main(int argc,
                      char **argv)
{
    int returnStatus = EXIT_FAILURE;

    /* A buffer containing the update document. It has static duration to prevent
     * it from being placed on the call stack. */

    (void)argc;
    (void)argv;

    do
    {
        /* code */
        returnStatus = EstablishMqttSession(eventCallback);
        if (returnStatus == EXIT_FAILURE)
        {
            LogWarn(("Error on connect. Retrying..."));
            sleep(5);
        }

    } while (returnStatus == EXIT_FAILURE);

    if (returnStatus == EXIT_SUCCESS)
    {
        LogInfo(("IoT code completed successfully."));
    }

    return returnStatus;
}

/*-----------------------------------------------------------*/
