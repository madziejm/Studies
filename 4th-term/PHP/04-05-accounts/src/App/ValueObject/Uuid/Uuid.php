<?php

namespace App\ValueObject\Uuid;

//require 'vendor/autoload.php';
use Exception;


/**
 * Class Uuid
 * @package App\ValueObject\Uuid
 */
class Uuid
{

    /**
     * @string
     */
    protected $value;

    /**
     * @return string|null
     */
    public function getValue()
    {
        return $this->value;
    }

    /**
     * Uuid constructor.
     * @param string|null $value
     * @throws Exception
     */
    public function __construct(?string $value = null)
    {
        if ($value == null)
            $this->value = \Ramsey\Uuid\Uuid::uuid4()->toString();
        else
            $this->value = \Ramsey\Uuid\Uuid::fromString($value)->toString();
    }

    public function __toString()
    {
        return $this->getValue();
    }
}