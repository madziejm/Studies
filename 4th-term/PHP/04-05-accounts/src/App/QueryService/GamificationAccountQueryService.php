<?php


namespace App\QueryService;


use App\DataTransferObject\GamificationAccountDTO;
use App\GamificationAccount;
use App\Repository\GamificationAccountRepositoryInterface;
use App\ValueObject\Uuid\GamificationAccountUuid;

/**
 * Class GamificationAccountQueryService
 * @package App\QueryService
 */
class GamificationAccountQueryService implements GamificationAccountQueryServiceInterface
{
    /**
     * @var GamificationAccountRepositoryInterface
     */
    private $gamificationAccountRepository;

    public function __construct($gamificationAccountRepository)
    {
        $this->gamificationAccountRepository = $gamificationAccountRepository;
    }

    public function findAllAccounts(): array
    {
        $gamificationAccounts = $this->gamificationAccountRepository->findAllAccounts();

        return array_map(
            function (GamificationAccount $gamificationAccount) {
                return $gamificationAccount->toGamificationAccountDTO();
            },
            $gamificationAccounts);
    }

    public function findAccountByUuid(GamificationAccountUuid $uuid): GamificationAccountDTO
    {
        return $this->gamificationAccountRepository->findAccountByUuid($uuid)->toGamificationAccountDTO();
    }
}