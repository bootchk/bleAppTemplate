


#include <cassert>
#include "nrf_delay.h"

// Facades
#include "provisioner.h"
#include "nrfLog.h"


#include "softdevice.h"



static void provisioningFailedCallback() {
	NRFLog::log("provision fail");
}

static void provisioningSuccededCallback() {
	// TODO pass the provisioned value
	NRFLog::log("provision succeed");
}






/*
 * Using Provisioner
 * Simple delay between provisioning
 */
int main2(void)
{
	NRFLog::enable();

	Provisioner::init(provisioningSuccededCallback, provisioningFailedCallback);

	/*
	 * Start LongClock so we we can use Timer,
	 * even when SD is disabled.
	 * Provisioner uses Timer to elapse provisioning.
	 *
	 * Provisioner knows how to start clocks compatible with SD
	 */
	Provisioner::startClocks();

	while(true) {

		// Here you do "normal" app, including using radio if not provisioning

		NRFLog::log("Start provisioning");
		Provisioner::provisionWithSleep();
		NRFLog::log("End provisioning");

		// simple spin delay between provisionings
		nrf_delay_ms(4000);
	}
}

