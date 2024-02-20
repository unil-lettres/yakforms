<?php
/**
 * @file
 * Contains utility class.
 */

/**
 * @file
 * This class represents utility methods.
 */
require 'miniorange_ldap_support.php';
class MiniorangeLdapUtility {

  public static function isLDAPInstalled() {
    return (in_array('ldap', get_loaded_extensions()));
  }

  /**
   * This function adds the support form on the tabs.
   */
  public static function AddSupportButton(array &$form, $form_state)
    {
      $form['markup_idp_attr_header_top_support'] = array('#markup' => '</div><div class="mo_ldap_table_layout_support_1 mo_saml_container">',);

      $form['markup_support_1'] = array(
      '#markup' => '<h3><b>Feature Request/Contact Us:</b></h3><div><i>Need any help? We can help you with configuring your LDAP Server. Just send us a query and we will get back to you soon.<br /></i></div><br>',
      );

      $form['miniorange_ldap_email_address'] = array(
      '#type' => 'textfield',
      '#attributes' => array('style' => 'width:100%','placeholder' => 'Enter your email'),
      '#default_value' => variable_get('miniorange_saml_customer_admin_email'),
      );

      $form['miniorange_ldap_phone_number'] = array(
      '#type' => 'textfield',
      '#attributes' => array('style' => 'width:100%','placeholder' => 'Enter your phone number'),
      '#default_value' => variable_get('miniorange_saml_customer_admin_phone'),
      );

      $form['miniorange_ldap_support_query'] = array(
      '#type' => 'textarea',
      '#cols' => '10',
      '#rows' => '5',
      '#resizable' => FALSE,
      '#attributes' => array('style' => 'width:100%','placeholder' => 'Write your query here'),
      );

      $form['miniorange_ldap_support_submit_click'] = array(
      '#type' => 'submit',
      '#value' => t('Submit Query'),
      '#id' => 'button_config_center',
      '#limit_validation_errors' => array(),
      '#submit' => array('send_support_query'),
      );

      $form['miniorange_saml_support_note'] = array(
      '#markup' => '<div><br/>If you want custom features in the module, just drop an email to <a href="mailto:drupalsupport@xecurify.com">drupalsupport@xecurify.com</a></div></div>'
      );
    }

        /**
     * This function sends the support query
     */
    public static function send_query($email, $phone, $query)
    {
        if(empty($email)||empty($query)){
            if(empty($email)) {
                drupal_set_message(t('The <b>Email Address</b> field is required.'), 'error');
            }
            if(empty($query)) {
                drupal_set_message(t('The <b>Query</b> field is required.'), 'error');
            }
            return;
        }
        if (!valid_email_address($email)) {
            drupal_set_message(t('The email address <b><u>' . $email . '</u></b> is not valid.'), 'error');
            return;
        }
        if(empty($phone))
            $phone = variable_get('miniorange_ldap_customer_admin_phone');
        $support = new MiniOrangeLDAPSupport($email, $phone, $query);
        $support_response = $support->sendSupportQuery();
        if($support_response) {
            drupal_set_message(t('Support query sent successfully. We will get back to you shortly!'));
        }
        else {
            drupal_set_message(t('Error sending support query'), 'error');
        }
    }

  /**
   * Encrypt.
   */
  public static function encrypt($str) {
    $key = variable_get('miniorange_ldap_customer_admin_token', NULL);
    $block = mcrypt_get_block_size('rijndael_128', 'ecb');
    $pad = $block - (strlen($str) % $block);
    $str .= str_repeat(chr($pad), $pad);
    return base64_encode(mcrypt_encrypt(MCRYPT_RIJNDAEL_128, $key, $str, MCRYPT_MODE_ECB));
  }

  /**
   * Check if cURL is installed.
   */
  public static function isCurlInstalled() {
    if (in_array('curl', get_loaded_extensions())) {
      return 1;
    }
    else {
      return 0;
    }
  }

  /**
   * Check if an extension is installed.
   */
  public static function isExtensionInstalled($name) {
    if (in_array($name, get_loaded_extensions())) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * This function add debug logs.
   */
  public static function addLogger($log_info, $log_val='') {
    $enable_logs = variable_get('miniorange_ldap_enable_logs');
    if($enable_logs) {
      watchdog('ldap_auth',$log_info.'<pre><code>' . print_r($log_val, TRUE) . '</code></pre>');
    }
  }
}
