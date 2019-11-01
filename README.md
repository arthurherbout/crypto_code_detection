# Development of automatic techniques for identification of cryptographic code at scale

_Authors_: Hadrien Cornier, Redouane Dziri, Arthur Herbout, Corentin Llorca, Arnaud Stiegler

_Contributors_: George Argyros, Eleni Drinea, Michael McDougall

*This is an ongoing project*

## Our goal

Cryptography has taken a major role in today's way of life, as it is the main tool used nowadays to ensure that our everyday online communication is done safely. It also appears in critical areas such as banking, voting and privacy, as well as attracting some attention lately through the recent rise of cryptocurrencies such as bitcoin. Pieces of software that wish to include cryptography generally do so through some established cryptography libraries, such as OpenSSL; however, there are still some custom-made implementations, which deserve some extra scrutiny. Indeed, such custom implementations might present a huge security risk when not done properly: this was for instance the case with the WEP Wi-fi security algorithm, which was once very widely used in home networks around the world but was eventually shown to be crackable in less than 3 minutes.

The problem then lies with this question: how can we automatically detect custom-made cryptographic code from "regular" code? Indeed, it is currently possible for human readers to manually separate the two, but at a great cost in terms of time, given the size and the number of files that a single software package can have. Our aim is then to develop an automatic technique that could have similar levels of reliability to a human reader, but that could be deployed at scale. 

## Repository Structure

The `data` folder contains all collected data to train and test our tool on. It is divided in 4 sub-folders:
- `code-jam`
- `crypto-competitions`
- `crypto-library`
- `others`
containing data collected from different type of sources.
`test.json` and `train.json` contain the training and testing datasets.

The `exploration` folder contains some data visualizations on built features and raw data as well as plots exploring built models.

The `feature_engineering` folder contains everything that builds features from the raw files to run models on.

The `playground` folder exists for development purposes and contains experiments and code that awaits proper refactoring into the project architecture.

## Bibliography

_TODO_
