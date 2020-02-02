# DnDice
D&D format command line dice roller and statistics generator


## Installation
On Linux run the following in the dndice directory, it requires root access:

	./install.sh

## Usage
Once installed, DnDice can be used by running:

	dndice [options] [dice]

### Dice Formatting
Dice are formatted in the same style as in D&D and many other RPGs. Meaning, `NdM` means N M sided dice are rolled and their results summed. Multiple set of dice and constants can be added with `+`, subtracted with `-`, and multiplied with `*` or `x` placed between them.

`+N` or `-N` will be assumed to be `1d20+N` or `1d20-N`.

### Options
`--version`
Prints version information.

`--help` or `-h`
Prints a help menu.

`--quiet` or `-q`
Causes the program to prints minimal infomation, generally only the results or rolls.

`-n <number>`
Rolls the provided dice the number of times indicated, instead of once.

`--stats <method>` or `-s <method>`
 Generates character ability scores instead of rolling dice. The methods available:
* `std` Uses the standard ability score array provided in the D&D 5e Players Handbook.
* `d20` Rolls 1d20 for each ability score.
* `4d6` Rolls 4d6 and then sums the highest 3 for each ability score.

