Representing Debits and Credits
===============================

Raamatupidamine displays debit and credit values in transactions as a single integer, with positive values representing debits, and negative values representing credits. For example:

- A debit balance of £50.00 would be displayed as 50.00
- A credit balance of £713.60 would be displayed as -713.60

All monetary values are stored in the database as pennies, to avoid any precision issues using floating points.

| Debit  | Credit | Display | Internal |
|--------|--------|---------|----------|
| £56.70 |        | 56.70   | 5670     |
|        | £83.90 | -83.90  | -8390    |
