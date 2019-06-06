<?php
/**
 * Created by PhpStorm.
 * User: maciej
 * Date: 2019-06-05
 * Time: 23:34
 */

namespace Command;


abstract class Command
{
    /**
     * @var \DateTime
     */
    private $created;

    /**
     * Command constructor.
     */
    public function __construct()
    {
        $this->created = new \DateTime();
    }

    /**
     * @return \DateTime
     */
    public function getCreated()
    {
        return $this->created;
    }


}
