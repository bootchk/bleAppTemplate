
/*
 * version of main that uses timer between provisioning
 */

#include <cassert>

// Nordic SDK
#include "nrf_delay.h"

// Facades
#include "provisioner.h"
#include "nrfLog.h"
#include "softdevice.h"

// radioSoc
#include "sleeper.h"




namespace {


void timeToProvisionTimerHandler(TimerInterruptReason) {
	if ( Provisioner::isProvisioning() ) {
		/*
		 * Unexpected:
		 * Still provisioning from last timeout.
		 * Time between provisionings must be greater than provisioning expiration time.
		 */
		NRFLog::log("Err: time to provision but still provisioning");
	}
	else {
		NRFLog::log("start provisioner.");
		Provisioner::start();
	}
}


void startProvisionTimeTimer() {
	NRFLog::log("Start provisioning timer");

	// assert LongClock is running
	// assert SD disabled

	// Using my own timer class
	TimerAdaptor::start(4000, timeToProvisionTimerHandler);	// every second
}


void provisioningFailedCallback() {
	NRFLog::log("provision fail");

	// set alarm for next provisioning
	startProvisionTimeTimer();
}

void provisioningSuccededCallback() {
	// TODO pass the provisioned value
	NRFLog::log("provision succeed");

	// set alarm for next provisioning
	startProvisionTimeTimer();
}


}	// namespace



#ifdef APP_TIMER_TIME_TO_PROVISION

APP_TIMER is unreliable

// If need app_timer when SD is not running
#include "nrf_drv_clock.h"

if using app_timer for scheduling provisioning

APP_TIMER_DEF(provisionTimeTimerID);

static void timeToProvisionTimerHandler(void* context) {
	if ( Provisioner::isProvisioning() ) {
		/*
		 * Unexpected:
		 * Still provisioning from last timeout.
		 * Time between provisionings must be greater than provisioning expiration time.
		 */
		NRFLog::log("Err: time to provision but still provisioning");
	}
	else {
		NRFLog::log("start provisioner.");
		Provisioner::start();
	}
}

static void startProvisionTimeTimer() {
	NRFLog::log("Start provisioning timer");
	AppTimer::start(provisionTimeTimerID, 4000);	// every second
}


void startClocksAndTimers() {

    startLFClock();

    // Requires LFCLK running
    AppTimer::init();

    //AppTimer::createRepeating(&provisionTimeTimerID, timeToProvisionTimerHandler);
    AppTimer::createOneShot(&provisionTimeTimerID, timeToProvisionTimerHandler);

}
#endif



/*
 * Using Provisioner
 * Using a Timer between provisionings.
 */
int main3(void)
{
	NRFLog::enable();

	Provisioner::init(provisioningSuccededCallback, provisioningFailedCallback);

	/*
	 * Start LongClock so we we can use Timer,
	 * even when SD is disabled.
	 *
	 * Provisioner knows how to start clocks compatible with SD
	 */
	Provisioner::startClocks();


	/*
	 * Start timer until next time to provision.
	 * More generally, you provision subservient to your normal processing, say at regular intervals.
	 */
	startProvisionTimeTimer();
	/*
	 * assert single shot timer is started.
	 * When it expires, it starts provisioning.
	 */


	while(true) {

		// Here you do "normal" app, including using radio if not provisioning

		if (Provisioner::isProvisioning()) {
			NRFLog::log("Provisioning sleep");
			Provisioner::sleep();

			/*
			 * On wake:
			 * SD might be running still (events unrelated to Provisioner, such as clock rollover.)
			 *
			 * Provisioner might have finished (expired, or provisioned callback) and SD shutdown.
			 * In that case, provision time timer is armed again.
			 */
		}
		else {
			/*
			 *  Sleep until next time to provision.
			 *  Next event should be Timer timeout.
			 *  assert ProvisionTimeTimer is armed
			 */

			// Use directly __WFE and __SEV macros since SoftDevice is not enabled
			NRFLog::log("Nonprovisioning sleep");

			Sleeper::sleepUntilSpecificEvent(ReasonForWake::SleepTimerExpired);

			/*
			 * Assert there was only one timer, provision time timer.
			 * It expired and started provisioning.
			 */
		}
		// assert is provisioning or provision time timer is armed
	}
}
