# Linky Benchmark

Benchmarking tool for the Linky project

## Usage

Configure what you want to do in `config.env`. Then run :

    sudo ./bin/linky_benchmark

> [!WARNING]
> You have to run with `sudo` as RAPL and other probes require sudo rights. 

## Installation

Make sure that `turbostat` is installed and in your Path. Then, simply run :

    make

## TODO

- [x] add cpu affinity ro run in only one core
- [x] format output to enable post processing

FIO bencmark (changer pour construire de la lexture )
Désactiver des c states OK
Mesurer la fréquence périodique 
Est ce que il baisse la fréquence puis C?
Creuser inférence 

Faire une boucle avec des temps d'endormissement, de plus en plus grand etc (avec des sleeps, pour voir comment les bascules se font) caractériser par la mesure qaund et comment je bascule