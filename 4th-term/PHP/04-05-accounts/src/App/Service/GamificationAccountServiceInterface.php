<?php


namespace App\Service;


interface GamificationAccountServiceInterface
{
    //uuid <-> id
    public function createAccount(string $email, ?string $description): void;

    public function activateAccount(string $accountUuid): void;

    public function deactivateAccount(string $accountUuid): void;

    public function topUpAccount(string $accountUuid, int $amount): void;
}