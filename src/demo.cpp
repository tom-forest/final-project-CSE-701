#include "bigint.hpp"
#include <iostream>

using namespace std;

int main() {
    cout << "==========BIGINT DEMONSTRATOR V1==========\n";

    bigint earth_sun_distance_meters(149597870700);
    bigint sun_alpha_distance_ua(276445);
    bigint milkyway_radius_ua(4000000000);
    bigint local_group_radius_ua(632411000000);
    bigint virgo_diameter_ua(6940000000000);
    bigint universe_min_size_ly(93000000000);
    bigint ua_to_ly(63241);

    cout << "The mean distance between the earth and the sun is around " << earth_sun_distance_meters << " meters.\n";
    cout << "But that's still storable in a 64 bits variable !\n";
    cout << "The closest star, Alpha Centauri, is " << sun_alpha_distance_ua << " astronomical units away. \n";
    cout << "That's " << earth_sun_distance_meters * sun_alpha_distance_ua << " meters !\n";
    cout << "But to break the 64 bits limit, we must venture further out ! \n";
    cout << "Though hard to measure, the Milky Way's radius is believed to be close to " << milkyway_radius_ua * earth_sun_distance_meters << " meters.\n";
    cout << "Our home galaxy cluster's radius should be " << local_group_radius_ua * earth_sun_distance_meters << " meters.\n";
    cout << "The Virgo super-cluster it and we are part of could be " << virgo_diameter_ua * earth_sun_distance_meters << " meters wide.\n";
    cout << "Finally, the universe could be anything beyond " << ua_to_ly * universe_min_size_ly * earth_sun_distance_meters << " meters long but who knows ? Not us.";
}