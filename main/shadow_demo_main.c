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
 * @file shadow_demo_main.c
 *
 * @brief Demo for showing how to use the Device Shadow library's API. This version
 * of Device Shadow API provide macros and helper functions for assembling MQTT topics
 * strings, and for determining whether an incoming MQTT message is related to a
 * device shadow. The shadow can be either the classic shadow or a named shadow. Change
 * #SHADOW_NAME to select the shadow. The Device Shadow library does not depend on a MQTT library,
 * therefore the code for MQTT connections are placed in another file (shadow_demo_helpers.c)
 * to make it easy to read the code using Device Shadow library.
 *
 * This example assumes there is a powerOn state in the device shadow. It does the
 * following operations:
 * 1. Establish a MQTT connection by using the helper functions in shadow_demo_helpers.c.
 * 2. Assemble strings for the MQTT topics of device shadow, by using macros defined by the Device Shadow library.
 * 3. Subscribe to those MQTT topics by using helper functions in shadow_demo_helpers.c.
 * 4. Publish a desired state of powerOn by using helper functions in shadow_demo_helpers.c.  That will cause
 * a delta message to be sent to device.
 * 5. Handle incoming MQTT messages in eventCallback, determine whether the message is related to the device
 * shadow by using a function defined by the Device Shadow library (Shadow_MatchTopicString). If the message is a
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
#include "shadow_demo_helpers.h"

/* Shadow config include. */
#include "shadow_config.h"

/* SHADOW API header. */
#include "shadow.h"

/* JSON API header. */
#include "core_json.h"

/* Clock for timer. */
#include "clock.h"

#define HELLO_DOCUMENT \
    "{"                \
    "\"state\":{"      \
    "\"reported\":{"   \
    "\"powerOn\":1"    \
    "}"                \
    "}"                \
    "}"

/*-----------------------------------------------------------*/

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
 * incoming messages. This function demonstrates how to use the Shadow_MatchTopicString
 * function to determine whether the incoming message is a device shadow message
 * or not. If it is, it handles the message depending on the message type.
 */
static void eventCallback(MQTTContext_t *pMqttContext,
                          MQTTPacketInfo_t *pPacketInfo,
                          MQTTDeserializedInfo_t *pDeserializedInfo)
{
    ShadowMessageType_t messageType = ShadowMessageTypeMaxNum;
    const char *pThingName = NULL;
    uint8_t thingNameLength = 0U;
    const char *pShadowName = NULL;
    uint8_t shadowNameLength = 0U;
    uint16_t packetIdentifier;

    (void)pMqttContext;

    assert(pDeserializedInfo != NULL);
    assert(pMqttContext != NULL);
    assert(pPacketInfo != NULL);

    packetIdentifier = pDeserializedInfo->packetIdentifier;
    LogInfo(("mensagem X"));
}
/*-----------------------------------------------------------*/

/**
 * @brief Entry point of shadow demo.
 *
 * This main function demonstrates how to use the macros provided by the
 * Device Shadow library to assemble strings for the MQTT topics defined
 * by AWS IoT Device Shadow. Named shadow topic strings differ from unnamed
 * ("Classic") topic strings as indicated by the tokens within square brackets.
 *
 * The main function uses these macros for topics to subscribe to:
 * - SHADOW_TOPIC_STR_UPDATE_DELTA for "$aws/things/thingName/shadow[/name/shadowname]/update/delta"
 * - SHADOW_TOPIC_STR_UPDATE_ACC for "$aws/things/thingName/shadow[/name/shadowname]/update/accepted"
 * - SHADOW_TOPIC_STR_UPDATE_REJ for "$aws/things/thingName/shadow[/name/shadowname]/update/rejected"
 *
 * It also uses these macros for topics to publish to:
 * - SHADOW_TOPIC_STR_DELETE for "$aws/things/thingName/shadow[/name/shadowname]/delete"
 * - SHADOW_TOPIC_STR_UPDATE for "$aws/things/thingName/shadow[/name/shadowname]/update"
 *
 * The helper functions this demo uses for MQTT operations have internal
 * loops to process incoming messages. Those are not the focus of this demo
 * and therefore, are placed in a separate file shadow_demo_helpers.c.
 */
int aws_iot_demo_main(int argc,
                      char **argv)
{
    int returnStatus = EXIT_SUCCESS;
    // int demoRunCount = 0;

    /* A buffer containing the update document. It has static duration to prevent
     * it from being placed on the call stack. */
    // static char updateDocument[SHADOW_REPORTED_JSON_LENGTH + 1] = {0};

    (void)argc;
    (void)argv;

    // do
    // {
    returnStatus = EstablishMqttSession(eventCallback);

    if (returnStatus == EXIT_FAILURE)
    {
        /* Log error to indicate connection failure. */
        LogError(("Failed to connect to MQTT broker."));
    }
    else
    {
        /* Reset the shadow delete status flags. */
        // deleteResponseReceived = false;
        // shadowDeleted = false;

        /* First of all, try to delete any Shadow document in the cloud.
         * Try to subscribe to `/delete/accepted` and `/delete/rejected` topics. */
        // returnStatus = SubscribeToTopic(SHADOW_TOPIC_STR_DELETE_ACC(THING_NAME, SHADOW_NAME),
        //                                 SHADOW_TOPIC_LEN_DELETE_ACC(THING_NAME_LENGTH, SHADOW_NAME_LENGTH));

        returnStatus = SubscribeToTopic("$aws/things/saee/shadow/name/luz-gabriel",
                                        sizeof("$aws/things/saee/shadow/name/luz-gabriel"));

        // if (returnStatus == EXIT_SUCCESS)
        // {
        /* Try to subscribe to `/delete/rejected` topic. */
        // returnStatus = SubscribeToTopic(SHADOW_TOPIC_STR_DELETE_REJ(THING_NAME, SHADOW_NAME),
        //                                 SHADOW_TOPIC_LEN_DELETE_REJ(THING_NAME_LENGTH, SHADOW_NAME_LENGTH));
        // }
        LogInfo(("result %d", returnStatus));

        // returnStatus = PublishToTopic("$aws/things/saee/shadow/name/luz-gabriel",
        //                               sizeof("$aws/things/saee/shadow/name/luz-gabriel"),
        //                               HELLO_DOCUMENT,
        //                               sizeof(HELLO_DOCUMENT));

        if (returnStatus == EXIT_SUCCESS)
        {
            /* Publish to Shadow `delete` topic to attempt to delete the
             * Shadow document if exists. */
            // returnStatus = PublishToTopic("/luz/gabriel",
            //                               sizeof("/luz/gabriel"),
            //                               HELLO_DOCUMENT,
            //                               sizeof(HELLO_DOCUMENT));
            // LogInfo("documento publicado");
        }

        // /* Unsubscribe from the `/delete/accepted` and 'delete/rejected` topics.*/
        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     returnStatus = UnsubscribeFromTopic( SHADOW_TOPIC_STR_DELETE_ACC( THING_NAME, SHADOW_NAME ),
        //                                          SHADOW_TOPIC_LEN_DELETE_ACC( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ) );
        // }

        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     returnStatus = UnsubscribeFromTopic( SHADOW_TOPIC_STR_DELETE_REJ( THING_NAME, SHADOW_NAME ),
        //                                          SHADOW_TOPIC_LEN_DELETE_REJ( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ) );
        // }

        // /* Check if an incoming publish on `/delete/accepted` or `/delete/rejected`
        //  * topics. If a response is not received, mark the demo execution as a failure.*/
        // if( ( returnStatus == EXIT_SUCCESS ) && ( deleteResponseReceived != true ) )
        // {
        //     LogError( ( "Failed to receive a response for Shadow delete." ) );
        //     returnStatus = EXIT_FAILURE;
        // }

        // /* Check if Shadow document delete was successful. A delete can be
        //  * successful in cases listed below.
        //  *  1. If an incoming publish is received on `/delete/accepted` topic.
        //  *  2. If an incoming publish is received on `/delete/rejected` topic
        //  *     with an error code 404. This indicates that a delete was
        //  *     attempted when a Shadow document is not available for the
        //  *     Thing. */
        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     if( shadowDeleted == false )
        //     {
        //         LogError( ( "Shadow delete operation failed." ) );
        //         returnStatus = EXIT_FAILURE;
        //     }
        // }

        // /* Successfully connect to MQTT broker, the next step is
        //  * to subscribe shadow topics. */
        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     returnStatus = SubscribeToTopic( SHADOW_TOPIC_STR_UPDATE_DELTA( THING_NAME, SHADOW_NAME ),
        //                                      SHADOW_TOPIC_LEN_UPDATE_DELTA( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ) );
        // }

        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     returnStatus = SubscribeToTopic( SHADOW_TOPIC_STR_UPDATE_ACC( THING_NAME, SHADOW_NAME ),
        //                                      SHADOW_TOPIC_LEN_UPDATE_ACC( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ) );
        // }

        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     returnStatus = SubscribeToTopic( SHADOW_TOPIC_STR_UPDATE_REJ( THING_NAME, SHADOW_NAME ),
        //                                      SHADOW_TOPIC_LEN_UPDATE_REJ( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ) );
        // }

        // /* This demo uses a constant #THING_NAME and #SHADOW_NAME known at compile time therefore
        //  * we can use macros to assemble shadow topic strings.
        //  * If the thing name or shadow name is only known at run time, then we could use the API
        //  * #Shadow_AssembleTopicString to assemble shadow topic strings, here is the example for /update/delta:
        //  *
        //  * For /update/delta:
        //  *
        //  * #define SHADOW_TOPIC_MAX_LENGTH  (256U)
        //  *
        //  * ShadowStatus_t shadowStatus = SHADOW_SUCCESS;
        //  * char topicBuffer[ SHADOW_TOPIC_MAX_LENGTH ] = { 0 };
        //  * uint16_t bufferSize = SHADOW_TOPIC_MAX_LENGTH;
        //  * uint16_t outLength = 0;
        //  * const char thingName[] = { "TestThingName" };
        //  * uint16_t thingNameLength  = ( sizeof( thingName ) - 1U );
        //  * const char shadowName[] = { "TestShadowName" };
        //  * uint16_t shadowNameLength  = ( sizeof( shadowName ) - 1U );
        //  *
        //  * shadowStatus = Shadow_AssembleTopicString( ShadowTopicStringTypeUpdateDelta,
        //  *                                            thingName,
        //  *                                            thingNameLength,
        //  *                                            shadowName,
        //  *                                            shadowNameLength,
        //  *                                            & ( topicBuffer[ 0 ] ),
        //  *                                            bufferSize,
        //  *                                            & outLength );
        //  */

        // /* Then we publish a desired state to the /update topic. Since we've deleted
        //  * the device shadow at the beginning of the demo, this will cause a delta message
        //  * to be published, which we have subscribed to.
        //  * In many real applications, the desired state is not published by
        //  * the device itself. But for the purpose of making this demo self-contained,
        //  * we publish one here so that we can receive a delta message later.
        //  */
        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     /* desired power on state . */
        //     LogInfo( ( "Send desired power state with 1." ) );

        //     ( void ) memset( updateDocument,
        //                      0x00,
        //                      sizeof( updateDocument ) );

        //     /* Keep the client token in global variable used to compare if
        //      * the same token in /update/accepted. */
        //     clientToken = ( Clock_GetTimeMs() % 1000000 );

        //     snprintf( updateDocument,
        //               SHADOW_DESIRED_JSON_LENGTH + 1,
        //               SHADOW_DESIRED_JSON,
        //               ( int ) 1,
        //               ( long unsigned ) clientToken );

        //     returnStatus = PublishToTopic( SHADOW_TOPIC_STR_UPDATE( THING_NAME, SHADOW_NAME ),
        //                                    SHADOW_TOPIC_LEN_UPDATE( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ),
        //                                    updateDocument,
        //                                    ( SHADOW_DESIRED_JSON_LENGTH + 1 ) );
        // }

        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     /* Note that PublishToTopic already called MQTT_ProcessLoop,
        //      * therefore responses may have been received and the eventCallback
        //      * may have been called, which may have changed the stateChanged flag.
        //      * Check if the state change flag has been modified or not. If it's modified,
        //      * then we publish reported state to update topic.
        //      */
        //     if( stateChanged == true )
        //     {
        //         /* Report the latest power state back to device shadow. */
        //         LogInfo( ( "Report to the state change: %"PRIu32"", currentPowerOnState ) );
        //         ( void ) memset( updateDocument,
        //                          0x00,
        //                          sizeof( updateDocument ) );

        //         /* Keep the client token in global variable used to compare if
        //          * the same token in /update/accepted. */
        //         clientToken = ( Clock_GetTimeMs() % 1000000 );

        //         snprintf( updateDocument,
        //                   SHADOW_REPORTED_JSON_LENGTH + 1,
        //                   SHADOW_REPORTED_JSON,
        //                   ( int ) currentPowerOnState,
        //                   ( long unsigned ) clientToken );

        //         returnStatus = PublishToTopic( SHADOW_TOPIC_STR_UPDATE( THING_NAME, SHADOW_NAME ),
        //                                        SHADOW_TOPIC_LEN_UPDATE( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ),
        //                                        updateDocument,
        //                                        ( SHADOW_REPORTED_JSON_LENGTH + 1 ) );
        //     }
        //     else
        //     {
        //         LogInfo( ( "No change from /update/delta, unsubscribe all shadow topics and disconnect from MQTT.\r\n" ) );
        //     }
        // }

        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     LogInfo( ( "Start to unsubscribe shadow topics and disconnect from MQTT. \r\n" ) );
        //     returnStatus = UnsubscribeFromTopic( SHADOW_TOPIC_STR_UPDATE_DELTA( THING_NAME, SHADOW_NAME ),
        //                                          SHADOW_TOPIC_LEN_UPDATE_DELTA( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ) );
        // }

        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     returnStatus = UnsubscribeFromTopic( SHADOW_TOPIC_STR_UPDATE_ACC( THING_NAME, SHADOW_NAME ),
        //                                          SHADOW_TOPIC_LEN_UPDATE_ACC( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ) );
        // }

        // if( returnStatus == EXIT_SUCCESS )
        // {
        //     returnStatus = UnsubscribeFromTopic( SHADOW_TOPIC_STR_UPDATE_REJ( THING_NAME, SHADOW_NAME ),
        //                                          SHADOW_TOPIC_LEN_UPDATE_REJ( THING_NAME_LENGTH, SHADOW_NAME_LENGTH ) );
        // }

        /* The MQTT session is always disconnected, even there were prior failures. */
        // returnStatus = DisconnectMqttSession();
    }

    /* This demo performs only Device Shadow operations. If matching the Shadow
     * topic fails or there are failures in parsing the received JSON document,
     * then this demo was not successful. */
    //     if (eventCallbackError == true)
    //     {
    //         returnStatus = EXIT_FAILURE;
    //     }

    //     /* Increment the demo run count. */
    //     // demoRunCount++;

    //     if (returnStatus == EXIT_SUCCESS)
    //     {
    //         LogInfo(("Demo iteration %d is successful.", demoRunCount));
    //     }
    //     /* Attempt to retry a failed iteration of demo for up to #SHADOW_MAX_DEMO_LOOP_COUNT times. */
    //     else if (demoRunCount < SHADOW_MAX_DEMO_LOOP_COUNT)
    //     {
    //         LogWarn(("Demo iteration %d failed. Retrying...", demoRunCount));
    //         sleep(DELAY_BETWEEN_DEMO_RETRY_ITERATIONS_S);
    //     }
    //     /* Failed all #SHADOW_MAX_DEMO_LOOP_COUNT demo iterations. */
    //     else
    //     {
    //         LogError(("All %d demo iterations failed.", SHADOW_MAX_DEMO_LOOP_COUNT));
    //         break;
    //     }
    // } while (returnStatus != EXIT_SUCCESS);

    while (
        true)
    {
        sleep(5000);
    }

    // if (returnStatus == EXIT_SUCCESS)
    // {
    //     /* Log message indicating the demo completed successfully. */
    //     LogInfo(("Demo completed successfully."));
    // }

    return returnStatus;
}

/*-----------------------------------------------------------*/
