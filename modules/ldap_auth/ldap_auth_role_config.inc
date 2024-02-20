<?php

function ldap_auth_role_mapping($form, $form_state)
{
    global $base_url;
    drupal_add_css( drupal_get_path('module', 'ldap_auth'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'ldap_auth'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

    $form['mo_header_style'] = array('#markup' => '<div class="mo_ldap_table_layout_1">');

    $form['prefix'] = array('#markup' => '<strong style="color: red">Note: </strong>If you want to test any of our premium feature, you can request a demo by dropping us an email at <a href=\'mailto:drupalsupport@xecurify.com\'>drupalsupport@xecurify.com</a></strong><br><br>');

    $form['mo_top_div'] = array(
        '#markup' => '<div class="mo_ldap_table_layout mo_ldap_container">',
    );

    $form['markup_top_vt_start'] = array(
        '#markup' => '<b><span style="font-size: 17px;">User Role Mapping [<a href="'.$base_url.'/admin/config/people/ldap_auth/license">Premium</a>]</span></b><br><hr><br/>'
    );

    if(!MiniorangeLdapUtility::isLDAPInstalled()){
        $form['markup_reg_msg'] = array(
            '#markup' => '<div class="mo_ldap_enable_extension_message"><b>The PHP LDAP extension is not enabled.</b><br> Please Enable the PHP LDAP Extension for you server to continue. If you want, you refer to the steps given on the link  <a target="_blank" href="https://faq.miniorange.com/knowledgebase/how-to-enable-php-ldap-extension/" >here</a> to enable the extension for your server.</div><br>',
        );
    }
    $form['miniorange_enable_role_mapping'] = array(
        '#type' => 'checkbox',
        '#title' => t('<b>Enable Role Mapping</b>'),
        '#disabled' => TRUE,
        '#description' => t('Enabling Role Mapping will automatically map Users from LDAP Groups to below selected Drupal Role. Role mapping will not be applicable for primary admin of Drupal.'),
    );

    $mrole= user_roles($membersonly = TRUE);
    $drole = array_search(variable_get('miniorange_oauth_default_role',''),$mrole);

    $form['miniorange_ldap_default_mapping'] = array(
        '#type' => 'select',
        '#id' => 'miniorange_ldap_dropdown',
        '#title' => t('Default Role'),
        '#options' => $mrole,
        '#default_value' => $drole,
        '#attributes' => array('style' => 'width:73%;'),
        '#disabled' => TRUE,
        '#description' => t('Default role will be assigned to all users for which mapping is not specified.'),
    );

    $form['miniorange_ldap_username_attr'] = array(
        '#type' => 'textfield',
        '#title' => t('LDAP Group Name: '),
        '#disabled' => TRUE,
        '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'memberOf'),
        '#description' => t('Specify attribute which stores group names to which LDAP Users belong.'),
    );

    $form['miniorange_ldap_role'] = array(
            '#type' => 'textfield',
            '#prefix' => '<table style="border: unset"><tr><td style="width: 51%;">',
            '#suffix' => '</td>',
            '#id' => 'text_field',
            '#title' => t('LDAP Group Name'),
            '#attributes' => array('style' => 'width:50%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'cn=group,dc=domain,dc=com'),
            '#disabled' => TRUE,
        );
        $form['miniorange_ldap_drupal_role'] = array(
            '#type' => 'select',
            '#id' => 'text_field',
            '#prefix' => '<td style="width: 32%;">',
            '#suffix' => '</td>',
            '#title' => t('Drupal Role'),
            '#options' => $mrole,
            '#default_value' => $drole,
            '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;'),
            '#disabled' => TRUE,
        );
        $form['miniorange_ldap_add_name'] = array(
            '#prefix' => '<td>',
            '#suffix' => '</td>',
            '#type' => 'button',
            '#disabled' => 'true',
            '#attributes' => array('style' => 'background-color: lightgreen;color:white'),
            '#value' => '+',
        );
        $form['miniorange_ldap_sub_name'] = array(
            '#prefix' => '<td style="border: unset">',
            '#suffix' => '</td></tr></table>',
            '#type' => 'button',
            '#disabled' => 'true',
            '#attributes' => array('style' => 'background-color: red;color:white'),
            '#value' => '-',
        );

    $form['miniorange_ldap_attribute_test_config'] = array(
        '#type' => 'submit',
        '#suffix' => '<br>',
        '#value' => t('Save Mapping'),
        '#disabled' => TRUE,
        '#id' => 'button_config',
    );

    $form['markup_top_vt_start1'] = array(
        '#markup' => '<br><b><span style="font-size: 17px;">Test Role Mapping Configuration</span></b><br><hr><br/>'
    );

    $form['miniorange_ldap_username_attr1'] = array(
        '#type' => 'textfield',
        '#title' => t('Username: '),
        '#disabled' => TRUE,
        '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Username'),
        '#description' => t('Enter LDAP username to test role mapping configurations.'),
    );

    $form['miniorange_ldap_attribute_test_config1'] = array(
        '#type' => 'submit',
        '#value' => t('Test Configurations'),
        '#disabled' => TRUE,
        '#id' => 'button_config',
    );

    MiniorangeLdapUtility::AddSupportButton($form, $form_state);

    return $form;
}

function send_support_query(&$form, $form_state)
    {
        $email = trim($form['miniorange_ldap_email_address']['#value']);
        $phone = $form['miniorange_ldap_phone_number']['#value'];
        $query = trim($form['miniorange_ldap_support_query']['#value']);
        MiniorangeLdapUtility::send_query($email, $phone, $query);
    }
?>
