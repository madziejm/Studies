<?php


namespace App\DataTransferObject;


use App\ValueObject\Email;
use App\ValueObject\Uuid\GamificationAccountUuid;
use Exception;
use JsonSerializable;

class GamificationAccountDTO implements JsonSerializable
{
    /** @var GamificationAccountUuid */
    private $uuid;

    /** @var Email */
    private $email;

    /** @var string|null */
    private $description;

    /** @var int */
    private $balance;

    /** @var bool */
    private $status;

    /**
     * GamificationAccountDTO constructor.
     * @param GamificationAccountUuid $uuid
     * @param string $email
     * @param string|null $description
     * @param int|null $balance
     * @param bool|null $status
     */
    public function __construct(
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
        $this->balance = $balance;
        $this->status = $status;
    }


    /**
     * @return GamificationAccountUuid
     */
    public function getUuid(): GamificationAccountUuid
    {
        return $this->uuid;
    }

//    /**
//     * @return array
//     */
//    public function toArray(): array
//    {
//        return [
//            'uuid' => $this->uuid->getValue(),
//            'email' => $this->email->getValue(),
//            'description' => $this->description,
//            'balance' => $this->balance,
//            'status' => $this->status
//        ];
//    }

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

    /**
     * Specify data which should be serialized to JSON
     * @link https://php.net/manual/en/jsonserializable.jsonserialize.php
     * @return mixed data which can be serialized by <b>json_encode</b>,
     * which is a value of any type other than a resource.
     * @since 5.4.0
     */
    public function jsonSerialize()
    {
        return ['uuid' => $this->uuid->getValue(),
            'email' => $this->email->getValue(),
            'description' => $this->description,
            'balance' => $this->balance,
            'status' => $this->status];
    }
}
