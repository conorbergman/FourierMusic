#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "fft_1.hpp"
#include "ColorMap.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <math.h>

int main()
{
	int WINDOW_HEIGHT = 900, WINDOW_WIDTH = 1800;
	std::string sMusic_filename = "GodsCountry";
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML window");
	// Load a sound to sound buffer
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("res/sounds/"+sMusic_filename+".wav"))
		return EXIT_FAILURE;
	// Create sound and set its buffer
	sf::Sound sound;
	sound.setBuffer(buffer);
	// get sound data
	// sample count
	sf::Uint64 count = (unsigned int)buffer.getSampleCount();
	// sample rate
	unsigned int rate = buffer.getSampleRate();
	// samples array
	const sf::Int16* samples = buffer.getSamples();
	std::cout << "count:  " << count << "  rate: " << rate << std::endl;
	unsigned long nCurrent_chunk = 0;
	unsigned long nCurrent = 0;
	unsigned int nChunk = 4096; // 2 ^ 12
	float fChunk_rate = (float)(nChunk * 500.0f) / rate; // chunks per millisecond
	int nElapsed_time = 0;

	std::cout << "chunk rate: " << fChunk_rate << std::endl;

	double* fft_sample_r = (double*)malloc(sizeof(double) * nChunk); // FFT real
	double* fft_sample_i = (double*)calloc(nChunk, sizeof(double));  // FFT imaginary
	double* fft_line_values = (double*)malloc(sizeof(double) * nChunk / 2); // FFT line array
	bool bCalc_buffer = 1;
	unsigned int i = 0;

	sf::VertexArray line(sf::LinesStrip, nChunk);
	sf::VertexArray fft_line(sf::Triangles, 3 * nChunk * 2);
	// Play the sound
	sound.play();
	// Start the game loop
	while (window.isOpen())
	{
		nElapsed_time = sound.getPlayingOffset().asMilliseconds();
		// check if needed to calculate fourier transform
		if (bCalc_buffer) {
			// update sound event
			i = 0;
			while ((i < nChunk) && ((nCurrent + i) < count))
			{
				fft_sample_r[i] = samples[nCurrent + i];
				fft_sample_i[i] = 0;
				i++;
			}
			FFT(1, 12, fft_sample_r, fft_sample_i);
			bCalc_buffer = 0;
		}
		// check if time to draw chunk
		if (nElapsed_time >= (nCurrent_chunk * fChunk_rate))
		{
			/*
			// print out time in song
			int mins = nElapsed_time / 60000.0;
			int secs = (int)(nElapsed_time / 1000.0) % 60;
			int millis = nElapsed_time % 1000;
			std::cout << "\rPlaying " << sMusic_filename << "... " << std::setfill('0') << std::setw(2) << mins;
			std::cout << ":" << std::setfill('0') << std::setw(2) << secs;
			std::cout << ":" << std::setfill('0') << std::setw(3) << millis;
			std::cout << "      offset: " << std::fixed << std::setprecision(3) << nCurrent << " of " << count << std::flush;
			*/

			// draw sound wave line
			for (unsigned int k = 0; k < i; k++)
			{
				line[k].position = sf::Vector2f(((float)k * WINDOW_WIDTH) / nChunk, samples[nCurrent + k] / 1000.0 + WINDOW_HEIGHT / 4.0);
			}
			
			// draw fourier transform
			for (unsigned int k = 0; k < i; k++)
			{
				int r, g, b;
				unsigned int j = 6 * k;
				// constants
				float value = abs(fft_sample_r[k]);
				int MAX_VAL = 500;
				float floor = (7.0 * WINDOW_HEIGHT / 8.0);
				float y = floor - value;
				float near_x = ((float)k * WINDOW_WIDTH) / nChunk;
				float far_x = ((float)(k+1) * WINDOW_WIDTH) / nChunk;
				//sf::Color c((int)(y/8) % 255,(int)(y/3) % 255, (int)y % 255);
				ColorMap(value / MAX_VAL, &r, &g, &b);
				sf::Color c(r, g, b);
				// first triangle of rect
				fft_line[j].position = sf::Vector2f( near_x, y ); // top left
				fft_line[j + 1].position = sf::Vector2f( far_x, y ); // top right
				fft_line[j + 2].position = sf::Vector2f( far_x, floor ); // bottom right
				// second triangle of rect
				fft_line[j + 3] = fft_line[j]; // top left 
				fft_line[j + 4] = fft_line[j + 2]; // bottom right
				fft_line[j + 5].position = sf::Vector2f( near_x, floor ); // bottom left
				// set color of rectangle
				fft_line[j].color = c;
				fft_line[j + 1].color = c;
				fft_line[j + 2].color = c;
				fft_line[j + 3].color = c;
				fft_line[j + 4].color = c;
				fft_line[j + 5].color = c;
			}
			nCurrent += nChunk;
			nCurrent_chunk++;
			bCalc_buffer = 1;
		}
		

		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}
		// Clear screen
		window.clear();
		// Update the window
		window.draw(line);
		window.draw(fft_line);
		window.display();
	}


	free(fft_sample_i);
	free(fft_sample_r);
	free(fft_line_values);

	return EXIT_SUCCESS;
}
