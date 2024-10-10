#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int width = 1920;
int height = 1080;

int maxIter = 128;
double zoomIn = 1.0;
double minRe = -2.5, maxRe = 1;
double minIm = -1, maxIm = 1;

// creating the visualization
void resetWindowAndImage(RenderWindow &window, Image &image, Texture &texture, Sprite &sprite)
{
    window.create(VideoMode(width, height), "Mandelbrot");
    image.create(width, height);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
}

Color linear_interpolation(const Color &v, const Color &u, double a)
{
    auto const b = 1 - a;
    return Color(b * v.r + a * u.r, b * v.g + a * u.g, b * u.b + a * u.b);
}

int main(void)
{
    RenderWindow window(VideoMode(width, height), "Mandelbrot Set Explorer");
    Image image;
    image.create(width, height);
    Texture texture;
    Sprite sprite;
    Font font;
    font.loadFromFile("/System/Library/Fonts/Geneva.ttf");
    Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(Color::White);

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            // first interaction - moving up/down/left/right
            if (e.type == Event::KeyPressed)
            {
                double w = (maxRe - minRe) * 0.3;
                double h = (maxIm - minIm) * 0.3;

                if (e.key.code == Keyboard::Left)
                {
                    minRe -= w, maxRe -= w;
                }
                if (e.key.code == Keyboard::Right)
                {
                    minRe += w, maxRe += w;
                }
                if (e.key.code == Keyboard::Up)
                {
                    minIm -= h, maxIm -= h;
                }
                if (e.key.code == Keyboard::Down)
                {
                    minIm += h, maxIm += h;
                }

                // change Resolution - '+' to increase, '-' to decrease
                if (e.key.code == Keyboard::Add)
                {
                    width += 100;
                    height += 50;
                    resetWindowAndImage(window, image, texture, sprite);
                }
                if (e.key.code == Keyboard::Subtract)
                {
                    width = max(800, width - 100); // minimum resolution 800x600
                    height = max(600, height - 50);
                    resetWindowAndImage(window, image, texture, sprite);
                }
            }

            // the bigger the iteration, the clearer the image
            if (e.type == Event::MouseWheelScrolled)
            {
                if (e.MouseWheelScrolled)
                {
                    if (e.mouseWheelScroll.wheel == Mouse::VerticalWheel)
                    {
                        if (e.mouseWheelScroll.delta > 0)
                            maxIter *= 2;
                        else
                            maxIter /= 2;
                        if (maxIter < 1)
                            maxIter = 1;
                    }
                }
            }

            // zooming in n out
            if (e.type == Event::MouseButtonPressed)
            {
                auto zoom_x = [&](double z)
                {
                    double cr = minRe + (maxRe - minRe) * e.mouseButton.x / width;
                    double ci = minIm + (maxIm - minIm) * e.mouseButton.y / height;

                    double tminr = cr - (maxRe - minRe) / 2 / z;
                    maxRe = cr + (maxRe - minRe) / 2 / z;
                    minRe = tminr;

                    double tmini = ci - (maxIm - minIm) / 2 / z;
                    maxIm = ci + (maxIm - minIm) / 2 / z;
                    minIm = tmini;
                };

                if (e.mouseButton.button == Mouse::Left)
                {
                    zoom_x(5);
                    zoomIn *= 5;
                }
                if (e.mouseButton.button == Mouse::Right)
                {
                    zoom_x(1.0 / 5);
                    zoomIn /= 5;
                }
            }
        }

        window.clear();
        // better resolution - Mandelbrot calculation per pixel
#pragma omp parallel for
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
            {
                double cr = minRe + (maxRe - minRe) * x / width;
                double ci = minIm + (maxIm - minIm) * y / height;
                double re = 0, im = 0;
                int iter;
                for (iter = 0; iter < maxIter; iter++)
                {
                    double tr = re * re - im * im + cr;
                    im = 2 * re * im + ci;
                    re = tr;
                    if (re * re + im * im > 2 * 2)
                        break;
                }

                int r = 1.0 * (maxIter - iter) / maxIter * 0xff;
                int g = r, b = r;

                static const vector<Color> colors{
                    {60, 0, 85},     
                    {187, 232, 255}, 
                    {230, 155, 218}, 
                    {240, 240, 255}, 
                    {0, 2, 0},
                };

                static const auto max_color = colors.size() - 1;

                if (iter < maxIter)
                {
                    double log_zn = log(re * re + im * im) / 2;
                    double nu = log(log_zn / log(2)) / log(2);
                    iter = iter + 1 - nu;
                }
                double mu = 1.0 * iter / maxIter;
                mu *= max_color;
                auto i_mu = static_cast<size_t>(mu);
                auto color1 = colors[i_mu];
                auto color2 = colors[min(i_mu + 1, max_color)];
                Color c = linear_interpolation(color1, color2, mu - i_mu);

                image.setPixel(x, y, Color(c));
            }

        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);

        char str[100];
        sprintf(str, "Max. iteraciones: %d\nZoom: x%2.2lf\nResolution: %dx%d", maxIter, zoomIn, width, height);
        text.setString(str);
        window.draw(text);
        window.display();
    }
    return 0;
}
