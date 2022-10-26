#include "myhash.h"

std::string Hash(std::string input)
{
    std::stringstream hex_hash;

    // Starting primes
    unsigned int hex_arr[8] = {0x9b05688c, 0x510e527f, 0x5be0cd19, 0x6a09e667, 0x3c6ef372, 0xa54ff53a, 0xbb67ae85, 0x1f83d9ab};
    // Other primes for calculations
    const double primes[32] = {107, 5, 229, 23, 3, 73, 7, 113, 181, 131, 47, 139, 79, 241, 89, 257, 23, 101, 269, 151, 11, 163, 173, 193, 59, 199, 211, 223, 277, 239, 307, 2};
    const double max_prime = 307.0;
    for (size_t i = 0; i < input.length(); i++)
    {
        double ascii = (int)input[i];
        // Handling "negative" ascii values
        if (ascii < 0)
            ascii += 256;

        // Formula
        unsigned int x = (double(primes[(int)ascii % 32]) - ascii / 100.0) / double(max_prime) * pow(2, 32);

        // Bitwise operations
        for (int j = 0; j < 8; j++)
        {
            unsigned int temp = hex_arr[j];
            hex_arr[j] >>= 4;
            hex_arr[j] ^= x;
            hex_arr[j] ^= temp;
        }
    }
    // Pushing hex values from array of hexes into the stringstream and adds leading zeroes where needed to preserve 8 symbol hexes
    for (int i = 0; i < 8; i++)
    {
        hex_hash << std::setfill('0') << std::setw(8) << std::right << std::hex << hex_arr[i];
    }

    // Returning stringstream value converted into string
    return hex_hash.str();
}