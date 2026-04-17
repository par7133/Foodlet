<?php

/**
 * Copyright 2021, 2028 NuMode
 *
 * This file is part of Foodlet.
 *
 * Foodlet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foodlet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.  
 * 
 * You should have received a copy of the GNU General Public License
 * along with Foodlet. If not, see <https://www.gnu.org/licenses/>.
 *
 * class.cookie.inc
 * 
 * Foodlet Cookie class.
 *
 * @author Daniele Bonini <code@gaox.io>
 * @copyrights (c) 2016, 2028 NuMode     
 * @license https://opensource.org/licenses/GPL-3.0
 */

namespace numode\numode;

/**
 * Cookie
 *
 * Cookie class
 *
 * @package  Cookie
 * @author   Daniele Bonini <code@gaox.io>
 * @version  2.0
 * @access   public
 */
final class Cookie {
  
  const EXPIRE_ONEDAY = 86400;
  const EXPIRE_SEVENDAYS = 604800;
  const EXPIRE_THIRTYDAYS = 2592000;
  const EXPIRE_SIXMONTHS = 15768000;
  const EXPIRE_ONEYEAR = 31536000;
  const EXPIRE_LIFETIME = -1;

  /**
   * Default constructor
   * 
   * @return void
   */
  private function __construct()
  {
  }
  
  /**
   * Check if the given cookie exists
   *
   * @param string $name the name of the cookie
   * @return bool if the given cookie exist, true/false
   */
  static public function exists(string $name): bool
  {
    return isset($_COOKIE[COOKIE_PREFIX . $name]);
  }

  /**
   * Check if the given cookie is empty
   *
   * @param string $name the name of the cookie
   * @return bool if the given cookie is empty, true/false
   */
  static public function isEmpty(string $name): bool
  {
    if (!self::exists($name)) {
      return true;
    }
    return empty($_COOKIE[COOKIE_PREFIX . $name]);
  }

  /**
   * Get the value of the given cookie
   *
   * @param string $name the name of the cookie
   * @param string $default the default value
   * @return string the value of the cookie
   */
  static public function get(string $name, string $default = ""): string
  {
    return (self::exists($name) ? $_COOKIE[COOKIE_PREFIX . $name] : $default);
  }

  /**
   * Set the value of a given cookie
   * 
   * @param string $name the name of the cookie
   * @param string $value the value
   * @param mixed $expire the expiration time
   * @param string $path the path
   * @param mixed $domain the domain
   * @return bool if the cookie has been successfully set, true/false
   */
  static public function set(string $name, string $value, ?int $expire = self::EXPIRE_ONEYEAR, string $path = "/", $domain = false): bool
  {
    $retval = false;
    
    settype($subdomain, "string");
    
    if (!headers_sent())
    {
      if ($domain === false) {
        //$domain = $_SERVER['HTTP_HOST'];
        $domain = $_SERVER['HTTP_HOST'];
        $domain = str_replace("www.", PHP_STR, strtolower($domain));
        $isSubdomain = isSubdomainHost($subdomain);
        if ($isSubdomain) {
          $domain = str_replace($subdomain . ".", PHP_STR, $domain);
        }
      }
      
      if ($expire === self::EXPIRE_LIFETIME) {
        $expire = PHP_INT_MAX;
      } elseif (is_numeric($expire)) {
        $expire = time() + $expire;
      } else {
        $expire = strtotime($expire);
      }
      
      $retval = @setcookie(COOKIE_PREFIX . $name, $value, $expire, $path, $domain, false, true);
      if ($retval) {
        $_COOKIE[COOKIE_PREFIX . $name] = $value;
      }
    }
    
    return $retval;
  }

  /**
   * Delete the given cookie
   *
   * @param string $name the name of the cookie
   * @param string $path the path of the cookie
   * @param mixed $domain the domain of the cookie
   * @return bool if the cookie has been successfully deleted, true/false
   */
  static public function delete(string $name, string $path = "/", $domain = false): bool
  {
    $retval = false;
    
    settype($subdomain, "string");
    
    if (!headers_sent())
    {
      if (self::exists($name)) {
        
        if ($domain === false) {
          //$domain = $_SERVER['HTTP_HOST'];
          $domain = $_SERVER['HTTP_HOST'];
          $domain = str_replace("www.", PHP_STR, strtolower($domain));
          $isSubdomain = isSubdomainHost($subdomain);
          if ($isSubdomain) {
            $domain = str_replace($subdomain . ".", PHP_STR, $domain);
          }
        }

        $retval = @setcookie(COOKIE_PREFIX . $name, "", time() - 3600, $path, $domain, false, true);

        unset($_COOKIE[COOKIE_PREFIX . $name]);
      }
    }
    
    return $retval;
  }
}
