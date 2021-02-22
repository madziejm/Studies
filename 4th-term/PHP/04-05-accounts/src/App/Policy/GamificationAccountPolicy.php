<?php

namespace App\Policy;

use App\Repository\GamificationAccountRepositoryInterface;

/**
 * Class GamificationAccountPolicy
 * @package App\Policy
 */
class GamificationAccountPolicy implements GamificationAccountPolicyInterface
{
    /**
     * @var GamificationAccountRepositoryInterface
     */
    private $gamificationAccountRepository;

    /**
     * GamificationAccountPolicy constructor.
     * @param GamificationAccountRepositoryInterface $gamificationAccountRepository
     */
    public function __construct(GamificationAccountRepositoryInterface $gamificationAccountRepository)
    {
        $this->gamificationAccountRepository = $gamificationAccountRepository;
    }

    public function validEmail(string $email): bool
    {
        $accounts = $this->gamificationAccountRepository->findAllAccounts();
        if ($accounts != null) {
            foreach ($accounts as $account) {
                if ($email === $account->getEmail()->getValue())
                    return false;
            }
        }
        return true;
    }

    public function validDescription(?string $description): bool
    {
        if ($description == null)
            return true;
        return strlen($description) < 100;
    }
}