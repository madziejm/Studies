<?php


namespace App\Service;

require __DIR__ . "/../../../vendor/autoload.php";

use App\Exception\InvalidDescriptionException;
use App\Exception\InvalidEmailException;
use App\GamificationAccount;
use App\Policy\GamificationAccountPolicyInterface;
use App\Repository\GamificationAccountRepositoryInterface;

class GamificationAccountService implements GamificationAccountServiceInterface
{
    /**
     * @var GamificationAccountRepositoryInterface
     */
    private $gamificationAccountRepository;
    /**
     * @var GamificationAccountPolicyInterface
     */
    private $gamificationAccountPolicy;

    /**
     * GamificationAccountService constructor.
     * @param GamificationAccountRepositoryInterface $gamificationAccountRepository
     * @param GamificationAccountPolicyInterface $gamificationAccountPolicy
     */
    public function __construct(GamificationAccountRepositoryInterface $gamificationAccountRepository,
                                GamificationAccountPolicyInterface $gamificationAccountPolicy)
    {
        $this->gamificationAccountRepository = $gamificationAccountRepository;
        $this->gamificationAccountPolicy = $gamificationAccountPolicy;
    }

    public function createAccount(string $email, ?string $description): void
    {
        if (!$this->gamificationAccountPolicy->validEmail($email))
            throw new InvalidEmailException();
        if (!$this->gamificationAccountPolicy->validDescription($description))
            throw new InvalidDescriptionException();

        $account = GamificationAccount::create($email, $description);
        $this->gamificationAccountRepository->add($account);
    }

    public function activateAccount(string $accountUuid): void
    {
        $account = $this->gamificationAccountRepository->findAccountByUuid($accountUuid);
        $account->activate();
        $this->gamificationAccountRepository->update($account);
    }

    public function deactivateAccount(string $accountUuid): void
    {
        $account = $this->gamificationAccountRepository->findAccountByUuid($accountUuid);
        $account->deactivate();
        $this->gamificationAccountRepository->update($account);
    }

    public function topUpAccount(string $accountUuid, int $amount): void
    {
        $account = $this->gamificationAccountRepository->findAccountByUuid($accountUuid);
        $account->topUpPoints($amount);
        $this->gamificationAccountRepository->update($account);
    }
}