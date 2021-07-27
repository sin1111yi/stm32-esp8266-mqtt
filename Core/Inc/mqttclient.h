/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/

#ifndef __MQTTCLIENT_H
#define __MQTTCLIENT_H

#include "main.h"

#include <MQTTPacket.h>
#include "net_conf.h"

#include "usart.h"
#include "esp8266.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MQTT_KeepAliveInterval   60
#define MQTT_MaxWaitCount        0xffff
#define MQTT_PacketBuffSize      1024

#define MQTT_RecvEndFlag         USART2_RecvEndFlag

int mqtt_transport_sendPacketBuffer(uint8_t *buf,int buflen);
int mqtt_transport_getdata(uint8_t *buf, int buflen);
int mqtt_transport_open(uint8_t *host, int port);
int mqtt_transport_close(int sock);

uint8_t mqtt_ConnectServer(void);

#endif
