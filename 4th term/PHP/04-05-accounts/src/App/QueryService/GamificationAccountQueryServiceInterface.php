<?php


namespace App\QueryService;


use App\DataTransferObject\GamificationAccountDTO;
use App\GamificationAccount;
use App\ValueObject\Uuid\GamificationAccountUuid;

interface GamificationAccountQueryServiceInterface
{
    public function findAllAccounts(): array;

    public function findAccountByUuid(GamificationAccountUuid $uuid): GamificationAccountDTO;
//    public function findAccountByEmail() : GamificationAccountDTO;
}