#!/bin/sh

echo -n "-DELF_INTERP=\"\\\""
readelf -l /usr/bin/sh | grep -xoEe ".*[[]Requesting program interpreter: ([/][^ ].+)[]].*" | awk -F:\  '{print $2}' | awk -F] '{print $1}' | tr -dc [:print:]
echo -n "\\\"\""
