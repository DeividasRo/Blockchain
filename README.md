# Supaprastinta blokų grandinė

## Naudojimo instrukcija

1. Atsisiųskite programos failus
2. Komandinėje eilutėje nueikite į programos direktoriją
3. Sukompiliuokite sprendimą įvedę komandą `make`
4. Kad paleisti programą, atidarykite .exe failą arba įveskite komandą `bchain`
5. Norėdami sužinoti visas komandas įveskite komandą `help`

```
>help

GENUSERS <amount> - generate a specified amount of blockchain users (wallets).
GENTX <amount> - generate a specified amount of unconfirmed transactions in the transaction pool.
CREATEBLOCKS - initiate block mining until all transactions are confirmed.
LISTBLOCKTX <block-height> - list all transaction information of a specified block.
LISTTXPOOL - list all real time transaction information in the transaction pool.
GETUSERINFO <public-key> - display information about a specified user.
GETTXINFO <txid> - display information about a specified transaction
GETBLOCKINFO <block-height> - display information about a specified block.
GETUSERCOUNT - display a real time amount of users (wallets) in the blockchain.
GETBLOCKCOUNT - display a real time amount of blocks in the blockchain.
GETTXPOOLSIZE - display a real time amount of unconfirmed transactions in the transaction pool.
RESET - reset blockchain, clearing all of its data.
```

## Programos aprašymas

Programa leidžia vartotojui susimuliuoti supaprastintą blokų grandinę. Programos galimybės:
- Vartotojų generavimas
- Transakcijų generavimas 
- Blokų kūrimas ir kasimas 
- Betkurio vartotojo informacijos peržiūra
- Betkurios transakcijos informacijos peržiūra
- Betkurio bloko informacijos peržiūra
- Bendra blokų grandinės informacija (vartotojų, blokų, transakcijų skaičius)

Pagal nutylėjimą blokų kasimo sudėtingumas lygus 5, kiekviename bloke telpa iki 100 transakcijų.

Blokų grandinės informacija išsaugoma **.txt** failuose **/data** direktorijoje.


