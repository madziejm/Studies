<?php


namespace App\Repository;


use App\GamificationAccount;

interface GamificationAccountRepositoryInterface
{
    public function add(GamificationAccount $gamificationAccount): void;

    public function findAllAccounts(): array; //itemsarray?

    public function findAccountByUuid($uuid): GamificationAccount;

    public function update(GamificationAccount $gamificationAccount);
}