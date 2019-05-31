<?php

namespace App;

use App\DataTransferObject\GamificationAccountDTO;
use App\Exception\InactiveAccountException;
use App\ValueObject\Email;
use App\ValueObject\Uuid\GamificationAccountUuid;
use Exception;
use InvalidArgumentException;

class GamificationAccount
{
    /** @var GamificationAccountUuid */
    private $uuid;

    /** @var Email */
    private $email;

    /** @var string|null */
    private $description;

    /** @var int */
    private $balance = 0;

    /** @var bool */
    private $status = true;

    /**
     * GamificationAccount constructor.
     * @param GamificationAccountUuid $uuid
     * @param string $email
     * @param string|null $description
     * @param int|null $balance
     * @param bool|null $status
     */
    private function __construct(
        GamificationAccountUuid $uuid,
        string $email,
        ?string $description = null,
        ?int $balance = null,
        ?bool $status = null
    )
    {
        $this->uuid = $uuid;
        $this->email = new Email($email);
        $this->description = $description;

        if ($balance != null && $balance > 0) {
            $this->balance = $balance;
        }

        if ($status != null) {
            $this->status = $status;
        }
    }

    /**
     * @param string $email
     * @param string|null $description
     * @return GamificationAccount
     */
    public static function create(string $email, ?string $description): self
    {
        return new self(
            new GamificationAccountUuid(),
            $email,
            $description,
            );
    }

    /**
     * @param array $data
     * @return GamificationAccount
     * @throws Exception
     */
    public static function recreateFromArray(array $data): self
    {
        return new self(
            new GamificationAccountUuid($data['uuid']),
            $data['email'],
            $data['description'],
            (int)$data['balance'],
            (bool)$data['status']
        );
    }

    /**
     * @param int $points
     * @throws Exception
     */
    public function topUpPoints(int $points): void
    {
        if (!$this->status) {
            throw new InactiveAccountException();
        }

        if ($points < 0) {
            throw new InvalidArgumentException();
        }

        $this->balance += $points;
    }

    /**
     *
     */
    public function deactivate(): void
    {
        $this->status = false;
    }

    /**
     *
     */
    public function activate(): void
    {
        $this->status = true;
    }

    /**
     * @return GamificationAccountUuid
     */
    public function getUuid(): GamificationAccountUuid
    {
        return $this->uuid;
    }

    /**
     * @return array
     */
    public function toArray(): array
    {
        return [
            'uuid' => $this->uuid->getValue(),
            'email' => $this->email->getValue(),
            'description' => $this->description,
            'balance' => $this->balance,
            'status' => $this->status
        ];
    }

    /**
     * @return Email
     */
    public function getEmail(): Email
    {
        return $this->email;
    }

    /**
     * @return string|null
     */
    public function getDescription(): ?string
    {
        return $this->description;
    }

    /**
     * @return int
     */
    public function getBalance(): int
    {
        return $this->balance;
    }

    /**
     * @return bool
     */
    public function getStatus(): bool
    {
        return $this->status;
    }

    public function toGamificationAccountDTO(): GamificationAccountDTO
    {
        return new GamificationAccountDTO(
            $this->getUuid(),
            $this->getEmail()->getValue(),
            $this->getDescription(),
            $this->getBalance(),
            $this->getStatus()
        );
    }
}
