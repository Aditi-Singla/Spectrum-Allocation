# Spectrum-Allocation

#### Scenario: 

You are the telecom minister in the government of a developing country. You want to auction off construction of cell-phone towers to various cell-phone companies. You have divided the country into M regions and have asked the companies to bid prices for each region. However, the companies are smart. They know that their profits will increase if they can capture a whole state (or a set of nearby states) instead of just a smaller region within a state. So, instead of bidding per region they decide to bid on a *set* of regions at a time. Example, a company may say, that it will pay $10,000 if it gets to build towers in regions 1, 3 and 5, however, it will pay only $3,000 if it is allowed just regions 1 and 3. Your government wants to find which bids to accept (that is give which region to which company) in order to maximize its own profits. We can assume that all companies will do a high quality job and are equal in every other way. Your government has another goal – it wants to be fair to all companies, and so you have decided to not accept more than one bid for any company. Note that you may choose to not accept any bid of a company. Also assume that for the purpose of first allocation that some region may go undeveloped.

#### Problem Statement: 

There are M regions to be auctioned off. There are B bids and C companies. Each bid has a company id, a set of regions, and a dollar amount. Find which subset of bids to accept so that you maximize the total auction amount. The total auction amount is sum of dollar amounts of an accepted bid. Of course, you cannot allocate one region to two companies. And, you will not accept two bids of the same company.


#### Running the code

1. To compile -

   ```bash
   ./scripts/compile.sh
   ```

2. To run - 

   ```bash
   ./scripts/run.sh <input-file> <output-file>
   ```