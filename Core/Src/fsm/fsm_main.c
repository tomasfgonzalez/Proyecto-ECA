/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : fsm_main.c
  * @brief          : Main FSM file
  ******************************************************************************
  */

#include <stdbool.h>
#include "stm32l0xx_hal.h"

#include "fsm/fsm_main.h"
#include "fsm/fsm_link.h"
#include "fsm/fsm_transmit.h"
#include "fsm/fsm_transmit_backup.h"

static FSM_Main_State currentState = INIT;

// Transitions
static bool isInitError(void) {
  // TODO: Implement error check
  return false;
}

static void performShutdown(void) {
  // TODO: Implement shutdown procedure
}

static bool isInitSuccess(void) {
  // TODO: Implement link establishment check
  return false;
}

static bool isLinkEstablished(void) {
  // TODO: Implement link establishment check
  return false;
}

static bool isLinkError(void) {
  // TODO: Implement link error check
  return false;
}

static bool isLinkErrorResolved(void) {
  // TODO: Implement error resolution check
  return true;
}

static bool hasMainChannelFailed(void) {
  // TODO: Implement backup condition check
  return false;
}

static bool isBackupTransmissionComplete(void) {
  // TODO: Implement backup completion check
  return false;
}

void FSM_Main_handle(void) {
  switch (currentState) {
    case INIT:
      if (isInitError()) {
        currentState = INIT_ERROR;
      } else if (isInitSuccess()) {
        currentState = LINK;
      }
      break;

    case INIT_ERROR:
      performShutdown();
      break;

    case LINK:
      FSM_Link_handle();

      if (isLinkError()) {
        currentState = LINK_ERROR;
      } else if (isLinkEstablished()) {
        currentState = TRANSMIT;
      }
      break;

    case LINK_ERROR:
      if (isLinkErrorResolved()) {
        currentState = LINK;
      }
      break;

    case TRANSMIT:
      FSM_Transmit_handle();
      if (hasMainChannelFailed()) {
        currentState = TRANSMIT_BACKUP;
      }
      break;

    case TRANSMIT_BACKUP:
      FSM_TransmitBackup_handle();
      if (hasBackupChannelFailed()) {
        currentState = LINK_ERROR;
      } else if (isBackupTransmissionComplete()) {
        currentState = TRANSMIT;
      }
      break;

    default:
      currentState = INIT;
      break;
  }
}
