<?php


namespace App\Repository;


use App\GamificationAccount;
use FilesystemIterator;

class FileGamificationAccountRepository implements GamificationAccountRepositoryInterface
{
    private const DATA_DIRECTORY_PATH = __DIR__ . '/../../../public/data/';

    public function add(GamificationAccount $gamificationAccount): void
    {
        file_put_contents(self::DATA_DIRECTORY_PATH . $gamificationAccount->getUuid(),
            json_encode($gamificationAccount->toArray()));
    }

    public function findAllAccounts(): array
    {
        $fileSystemIterator = new FilesystemIterator(self::DATA_DIRECTORY_PATH);

        $accounts = [];

        foreach ($fileSystemIterator as $fileInfo) {
            $accounts[] = GamificationAccount::recreateFromArray(
                json_decode(
                    file_get_contents(
                        self::DATA_DIRECTORY_PATH . $fileInfo->getFilename()), true));
        }
        return $accounts;
    }

    public function findAccountByUuid($uuid): GamificationAccount
    {
        $json_file_contents = file_get_contents(self::DATA_DIRECTORY_PATH . $uuid);
//        if(!file_exists(self::DATA_DIRECTORY_PATH . $uuid))
//        TODO check this
        if (!$json_file_contents)
            return null;
        return GamificationAccount::recreateFromArray(json_decode($json_file_contents, true));
    }

    public function update(GamificationAccount $gamificationAccount)
    {
        file_put_contents(
            self::DATA_DIRECTORY_PATH . $gamificationAccount->getUuid(),
            json_encode($gamificationAccount->toArray())
        );
    }
}