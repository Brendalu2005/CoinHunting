#!/bin/bash

if [ -f /etc/os-release ]; then
    . /etc/os-release
    if [ "$ID" == "fedora" ]; then
        make -f Makefile.fedora
    elif [ "$ID" == "ubuntu" ] || [ "$ID" == "zorin" ]; then
        make -f Makefile
    else
        echo "Sistema não suportado"
    fi
else
    echo "sistema operacional não encontrado"
fi
