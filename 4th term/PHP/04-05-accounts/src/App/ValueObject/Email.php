<?php


namespace App\ValueObject;

use App\Exception\InvalidEmailException;

/**
 * Class Email
 * @package App\ValueObject
 */
class Email
{
    /**
     * @var string
     */
    private $value;

    /**
     * Email constructor.
     * @param string $value
     */
    public function __construct($value)
    {
        if (!filter_var($value, FILTER_VALIDATE_EMAIL))
            throw new InvalidEmailException();
        $this->value = $value;
    }

    /**
     * @return string
     */
    public function getValue()
    {
        return $this->value;
    }
}