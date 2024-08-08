#include "general.hpp"

sf::Color mixColors(std::vector<sf::Color> colors) 
{
    long double sum_r = 0, sum_g = 0, sum_b = 0, sum_a = 0;
    for(auto &c : colors)
    {
        sum_r += c.r;
        sum_g += c.g;
        sum_b += c.b;
        sum_a += c.a;
    }
    sum_r /= colors.size();
    sum_g /= colors.size();
    sum_b /= colors.size();
    sum_a /= colors.size();
    return sf::Color(sum_r, sum_g, sum_b, sum_a);
}

sf::Color colorGradiant(sf::Color c1, sf::Color c2, float distance) 
{
    sf::Color output;
    output.r = c1.r * (1 - distance) + c2.r * distance;
    output.g = c1.g * (1 - distance) + c2.g * distance;
    output.b = c1.b * (1 - distance) + c2.b * distance;
    output.a = c1.a * (1 - distance) + c2.a * distance;
    return output;
}
