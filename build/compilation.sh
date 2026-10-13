#!/usr/bin/env bash

if [[ $# -ne 1 ]]; then
	echo "USAGE: $0 <source.elf>"
	exit 1
fi



source_file="$1"

if [[ ! -f "$source_file" ]]; then
	echo "File does not exist: $source_file" >&2
	exit 1
fi


if [[ "$source_file" != *.elf ]]; then
	echo "ERROR: Needs a .c file" >&2
	exit 1
fi

output_base="${source_file%.elf}"
output_hex="${output_base}.hex"

echo "$output_base + $output_hex"
echo "The script is working for $source_file"



echo "Copying into $output_hex..."

avr-objcopy \
	 -j .data \
	 -j .text \
	 -O ihex \
	 "$source_file" \
	 "$output_hex"

 avrdude \
	 -v \
	 -p m328p \
	 -c arduino \
	 -P /dev/ttyUSB0 \
	 -b 115200 \
	 -U flash:w:"$output_hex":i
