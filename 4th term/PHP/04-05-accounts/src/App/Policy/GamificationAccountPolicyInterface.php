<?php


namespace App\Policy;


interface GamificationAccountPolicyInterface
{
    public function validEmail(string $email): bool;

    public function validDescription(string $description): bool;
}