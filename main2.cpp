


#include <cassert>
#include "nrf_delay.h"

// Facades
#include "provisioner.h"
#include "nrfLog.h"

#ifdef NOT_USING_PROVISIONER
// Provisioner manages SD and BLE
#include "service.h"
#include "adModule.h"
#include "uuid.h"
#include "softdevice.h"

#include "appTimer.h"
#include "gap.h"
#include "gatt.h"
#include "connection.h"
#endif


static void provisioningFailedCallback() {
	NRFLog::log("provision fail");
}

static void provisioningSuccededCallback() {
	// TODO pass the provisioned value
	NRFLog::log("provision succeed");
}


#ifdef APP_TIMER_TIME_TO_PROVISION

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



/*
 * Pointer to handler func inside a struct whose address is passed to ..._request when starting.
 */
static void clockStartedEventHandler(nrf_drv_clock_evt_type_t event) {
	// Called when clock is started.
	// We don't care when the clock starts, we know it will start and thus app_timers beging working.
	NRFLog::log("LF clock started");
}

nrf_drv_clock_handler_item_s handlers = {
		p_next: nullptr,
		event_handler: clockStartedEventHandler
};

static void startLFClock() {
	ret_code_t err_code;

	err_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(err_code);

	nrf_drv_clock_lfclk_request(nullptr);	// or (&handlers);

	// spin waiting for clock running stable
	while ( !nrf_drv_clock_lfclk_is_running() ) {
		;
	}
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
 */
int main(void)
{
    NRFLog::enable();

    Provisioner::init(provisioningSuccededCallback, provisioningFailedCallback);

    /*
     * App must coordinate use of hw clocks and timers with Provisioner.
     */
    Provisioner::startClocks();

    /*
     * Start timer until next time to provision.
     * More generally, you provision subservient to your normal processing, say at regular intervals.
     */



#ifdef APP_TIMER_TIME_TO_PROVISION
    startProvisionTimeTimer();
    // assert repeating timer is started
#endif


    while(true) {

    	// Here you do "normal" app, including using radio if not provisioning


#ifdef SLEEP_BETWEEN_PROVISIONINGS
    	if (Provisioner::isProvisioning()) {
    		NRFLog::log("Provisioning sleep");
    		Provisioner::sleep();

    		/*
    		 * On wake:
    		 * SD might be running still (events unrelated to Provisioner, such as clock rollover.)
    		 * Provisioner might have finished (expired, or provisioned callback) and SD shutdown.
    		 */
    	}
    	else {
    		// Sleep until next time to provision
    		// Use directly __WFE and __SEV macros since SoftDevice is not enabled
    		NRFLog::log("Nonprovisioning sleep");

    		// Clock must be running else timers not work
    		assert(nrf_drv_clock_lfclk_is_running());

    		// Wait for event.
    		__WFE();

    		// Clear Event Register.
    		__SEV();
    		__WFE();
#else
    		Provisioner::provisionWithSleep();

    		// simple spin delay between provisionings
    		nrf_delay_ms(4000);
#endif
    }
}
