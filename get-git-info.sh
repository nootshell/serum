#!/bin/sh

echo -n "-DGIT_BRANCH=\"\\\""
git rev-parse --abbrev-ref HEAD | tr -dc [:print:]
echo -n "\\\"\" -DGIT_COMMIT=\"\\\""
git rev-parse HEAD | tr -dc [:print:]
echo -n "\\\"\""
