<?php
include_once('ldap_auth_ldap_config.inc');
function ldap_auth_support($form, $form_state)
{
    global $base_url;
    $form['mo_header_style'] = array('#markup' => '<div class="mo_ldap_table_layout_1">');

    $form['prefix'] = array('#markup' => '<strong style="color: red">Note: </strong>If you want to test any of our premium feature, you can request a demo by dropping us an email at <a href=\'mailto:drupalsupport@xecurify.com\'>drupalsupport@xecurify.com</a></strong><br><br>');

    $form['mo_top_div'] = array(
        '#markup' => '<div class="mo_ldap_table_layout mo_ldap_container">',
    );

  $form['debug'] = array(
    '#type' => 'details',
    '#title' =>t('Debug Logs'),

  );
  $form['debug']['markup'] = array(
    '#markup' => '<h2>Debugging and Troubleshoot</h2><br>',
  );
  $form['debug']['loggers'] = array(
    '#type' => 'checkbox',
    '#name' => 'loggers',
    '#title' => t('Enable Logging '),
    '#description' => 'Enabling this checkbox will add loggers under the <a target = "_blank" href="'.$base_url.'/admin/reports/dblog?type%5B%5D=ldap_auth">Reports</a> section',
    '#default_value' => variable_get('miniorange_ldap_enable_logs',FALSE),
  );
  $form['debug']['miniorange_ldap_save_logs_option'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('save_logs_option'),
    '#prefix' => '<br>',
    '#suffix' => '<br>',
    '#id' => 'button_config',
  );

  $form['demo_support'] = array(
    '#type' => 'details',
    '#title' => "Request for a Demo",
    '#group' => 'ldap_debug',
  );

  $form['markup_support_1'] = array(
        '#markup' => '<br><hr><br><h2>Support/Feature Request</h2>Need any help? Just send us a query so we can help you.<br>',
    );

    $form['miniorange_ldap_email_address'] = array(
        '#type' => 'textfield',
        '#title' => t('Email Address'),
        '#attributes' => array('style' => 'width:70%','placeholder' => 'Enter your email'),
    );

    $form['miniorange_ldap_phone_number'] = array(
        '#type' => 'textfield',
        '#title' => t('Phone number'),
        '#attributes' => array('style' => 'width:70%','placeholder' => 'Enter your phone number'),
    );

    $form['miniorange_ldap_support_query'] = array(
        '#type' => 'textarea',
        '#title' => t('Query'),
        '#cols' => '10',
        '#rows' => '5',
        '#resizable' => false,
        '#attributes' => array('style' => 'width:70%','placeholder' => 'Write your query here'),
    );
    $form['miniorange_ldap_support_submit_click'] = array(
        '#type' => 'submit',
        '#value' => t('Submit Query'),
        '#submit' => array('send_support_query'),
        '#id' => 'button_config',
    );

    $form['miniorange_ldap_support_note'] = array(
        '#markup' => '<br><br>If you want custom features in the module, just drop an email to <a href="mailto:drupalsupport@xecurify.com">drupalsupport@xecurify.com</a></div></div>'
    );

    return $form;

    }

?>
