#include <cmath>
#include <iomanip>
#include <iostream>

const double PI = 3.1415926536;

double
area_of_circle(double radius)
{
    return radius * radius * PI;
}

double
area_of_annulus(double outer_radius, double inner_radius)
{
    return (outer_radius * outer_radius - inner_radius * inner_radius) * PI;
}

double
area_of_section_helper(double x)
{
    return 0.5 * (x + sin(x) * cos(x));
}

double
area_of_section(double radius, double x1, double x2, double y)
{
//    std::cerr << " [radius=" << radius << "; x1=" << x1 << "; x2=" << x2 << "; y=" << y << "] ";
    double ax1 = asin(x1 / radius);
    double ax2 = asin(x2 / radius);

    double c1 = radius * radius * (area_of_section_helper(ax2) - area_of_section_helper(ax1));
    double c2 = y * (x2 - x1);

//    std::cerr << " [c1=" << c1 << "; c2=" << c2 << "] ";

    return c1 - c2;
}

double
area_of_strings(double racquet_radius, double string_radius, double spacing)
{
    // find all the openings in a quadrant
    int n = (int)(racquet_radius / spacing + 1.0);
    double opening_radius = (spacing - 2 * string_radius) / 2;
    double racquet_radius_squared = racquet_radius * racquet_radius;
    double opening_radius_squared = opening_radius * opening_radius;

//    std::cerr << "*****************" << std::endl;
//    std::cerr << "Racquet radius: " << racquet_radius << std::endl;
//    std::cerr << "String radius: " << string_radius << std::endl;
//    std::cerr << "Spacing: " << spacing << std::endl;
//    std::cerr << "Opening radius: " << opening_radius << std::endl;
//    std::cerr << "# of grid points: " << n << std::endl;

    double total = 0.0;
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= n; ++j)
        {
//            std::cerr << "Checking grid point " << i << "," << j << ": ";
            double center_x, center_y;
            center_x = (i - 0.5) * spacing;
            center_y = (j - 0.5) * spacing;

            double closest_x, closest_y;
            closest_x = center_x - opening_radius;
            closest_y = center_y - opening_radius;
            if(closest_x * closest_x + closest_y * closest_y > racquet_radius_squared) {
//                std::cerr << "Outside racquet" << std::endl;
                continue;   // this opening is not within the racquet
            }

            double farthest_x, farthest_y;
            farthest_x = center_x + opening_radius;
            farthest_y = center_y + opening_radius;

            // locate corners
            bool upper_left, lower_right;
            bool upper_right = (farthest_x * farthest_x + farthest_y * farthest_y < racquet_radius_squared);
            if(upper_right) {
                upper_left = true;
                lower_right = true;
            } else {
                upper_left = (closest_x * closest_x + farthest_y * farthest_y < racquet_radius_squared);
                lower_right = (farthest_x * farthest_x + closest_y * closest_y < racquet_radius_squared);
            }

            double section_area;
            if(upper_right) {
//                std::cerr << "square section";
                section_area = 4 * opening_radius_squared;
            } else {
//                std::cerr << std::endl;
//                std::cerr << "(" << closest_x << "," << closest_y << ") ... (" << farthest_x << "," << farthest_y << ")" << std::endl;
                if(lower_right) {
                    if(upper_left) {
//                        std::cerr << "section has both LR and UL";
                        double temp_x = sqrt(racquet_radius_squared - farthest_y * farthest_y);
                        section_area = area_of_section(racquet_radius, temp_x, farthest_x, closest_y);
                        section_area += 2 * (temp_x - closest_x) * opening_radius;
                    } else {
//                        std::cerr << "section has LR only";
                        section_area = area_of_section(racquet_radius, closest_x, farthest_x, closest_y);
                    }
                } else {
                    if(upper_left) {
//                        std::cerr << "section has UL only";
                        double temp_x1 = sqrt(racquet_radius_squared - farthest_y * farthest_y);
                        double temp_x2 = sqrt(racquet_radius_squared - closest_y * closest_y);
                        section_area = area_of_section(racquet_radius, temp_x1, temp_x2, closest_y);
                        section_area += 2 * (temp_x1 - closest_x) * opening_radius;
                    } else {
//                        std::cerr << "section has no corners";
                        double temp_x = sqrt(racquet_radius_squared - closest_y * closest_y);
                        section_area = area_of_section(racquet_radius, closest_x, temp_x, closest_y);
                    }
                }
            }

//            std::cerr << ": " << section_area << std::endl;
            total += section_area;
        }

    return area_of_circle(racquet_radius) - 4 * total;
}

double
prob(double fly_radius, double racquet_radius, double racquet_thickness, double string_radius, double gap)
{
    double annulus = area_of_annulus(racquet_radius, racquet_radius - racquet_thickness - fly_radius);
    double strings = area_of_strings(racquet_radius - racquet_thickness - fly_radius, string_radius + fly_radius, 2 * string_radius + gap);
    double total = area_of_circle(racquet_radius);

    return (annulus + strings) / total;
}

int
main(int argc, char *argv[])
{
    int ncases;
    std::cin >> ncases;
    for(int i = 1; i <= ncases; ++i)
    {
        double fly_radius, racquet_radius, racquet_thickness, string_radius, gap;
        std::cin >> fly_radius >> racquet_radius >> racquet_thickness >> string_radius >> gap;
        std::cout << "Case #" << i << ": ";
        std::cout << std::fixed << std::setprecision(6) << prob(fly_radius, racquet_radius, racquet_thickness, string_radius, gap) << std::endl;
    }
}
