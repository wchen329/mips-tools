//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>
#define GIGABYTE 1073741824

const std::string img_header = "PSDISKF001"; // PRISCAS SIMULATION DISK FORMAT ###

/* PRISCAS Disk Utility
 * A simple utility used for creating persistent disk images.
 *
 */
int main(int argc, char ** argv)
{
	bool size_specced = false;
	bool delay_specced = false;
	bool filename_specced = false;
	uint32_t disk_size = 0;
	uint32_t disk_delay = 0;
	std::string filename;
	const int8_t ZERO = 0;

	fprintf(stdout, "PRISCAS Disk Creation Utility\n");
	if(argc < 2)
	{
		fprintf(stdout, "Usage: pdcutil --size [SIZE] -o [FILENAME] [--delay] [TIME]\n");
		fprintf(stdout, "Where:\n\tSIZE is the disk size in kilobytes\n");
		fprintf(stdout, "\tSPEED is average disk transfer delay in milliseconds\n");
		fprintf(stdout, "\tFILENAME is the output file name.\n");
		return -1;
	}

	for(int argi = 1; argi < argc; argi++)
	{
		std::string current = argv[argi];
		int argipp = argi + 1;
		
		if(current == "--size")
		{
			if(argipp < argc)
			{
				size_specced = true;
				disk_size = atol(argv[argipp]);
			}
			else
			{
				fprintf(stderr, "--size option requires value\n");
			}
		}

		if(current == "-o")
		{
			if(argipp < argc)
			{
				filename_specced = true;
				filename = (argv[argipp]);
			}
			else
			{
				fprintf(stderr, "-o option requires value\n");
			}
		}

		if(current == "--speed")
		{
			if(argipp < argc)
			{
				delay_specced = true;
				disk_delay = atol(argv[argipp]);
			}
			else
			{
				fprintf(stderr, "--speed option requires value\n");
			}
		}
	}

	if(!filename_specced || !size_specced)
	{
		fprintf(stderr, "Required argument missing.\n");
		fprintf(stdout, "Usage: pdcutil --size [SIZE] -o [FILENAME] [--delay] [TIME]\n");
		return 1;
	}

	if(disk_size == 0)
	{
		fprintf(stderr, "--size value must be greater than 0\n");
		return 1;
	}

	if(disk_size > GIGABYTE)
	{
		fprintf(stderr, "Size reduced to 1 GB (the maximum size).\n");
		disk_size = GIGABYTE;
	}

	if(filename == "")
	{
		fprintf(stderr, "-o filename cannot be empty\n");
		return 1;
	}

	fprintf(stdout, "Size specified: %ld bytes\n", disk_size);
	fprintf(stdout, "Delay specified: %ld ms\n", disk_delay);
	fprintf(stdout, "Creating disk image %s...\n", filename.c_str()); 

	FILE * imgfile = fopen(filename.c_str(), "w");

	// Write a special header (10 bytes) to indicate the file type
	fwrite(img_header.c_str(), sizeof(char), 10, imgfile);

	// Write the size and delay next
	fwrite(&disk_size, sizeof(uint32_t), 1, imgfile);
	fwrite(&disk_delay, sizeof(uint32_t), 1, imgfile);

	// Now zero out the rest of the space
	for(unsigned long csize = 0; csize < disk_size; csize++)
	{
		fwrite(&ZERO, sizeof(int8_t), 1, imgfile);
	}
	
	// Close the file
	fclose(imgfile);

	fprintf(stdout, "Operation completed successfully.", filename.c_str()); 

	return 0;
}