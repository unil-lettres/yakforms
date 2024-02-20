<?php

function ldap_auth_signin($form, $form_state)
{

    global $base_url;
    drupal_add_css( drupal_get_path('module', 'ldap_auth'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'ldap_auth'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

    $form['mo_header_style'] = array('#markup' => '<div class="mo_ldap_table_layout_1">');

    $form['prefix'] = array('#markup' => '<strong style="color: red">Note: </strong>If you want to test any of our premium feature, you can request a demo by dropping us an email at <a href=\'mailto:drupalsupport@xecurify.com\'>drupalsupport@xecurify.com</a></strong><br><br>');

    $form['mo_top_div'] = array(
        '#markup' => '<div class="mo_ldap_table_layout mo_ldap_container">',
    );

    if(!MiniorangeLdapUtility::isLDAPInstalled()){
        $form['markup_reg_msg'] = array(
            '#markup' => '<div class="mo_ldap_enable_extension_message"><b>The PHP LDAP extension is not enabled.</b><br> Please Enable the PHP LDAP Extension for you server to continue. If you want, you refer to the steps given on the link  <a target="_blank" href="https://faq.miniorange.com/knowledgebase/how-to-enable-php-ldap-extension/" >here</a> to enable the extension for your server.</div><br>',
        );
    }
    $form['miniorange_ldap_enable_ntlm'] = array(
        '#type' => 'checkbox',
        '#disabled' => true,
        '#description' => 'Enabling NTLM login will protect your website through login with NTLM. Upgrade to the <a href="'.$base_url.'/admin/config/people/ldap_auth/license"><b> PREMIUM</b></a> version of the module to use this feature.',
        '#title' => t('Enable NTLM/Kerberos Login '),
        '#default_value' => variable_get('miniorange_ldap_enable_ntlm', ''),
    );

    $form['miniorange_ldap_image'] = array(
        '#markup' => '
            <br><h1>What is Microsoft NTLM?</h1><hr>
            <p>NTLM is the authentication protocol used on networks that include systems running the Windows operating system and on stand-alone systems.</p>
            <p>NTLM credentials are based on data obtained during the interactive logon process and consist of a domain name, a user name, and a one-way hash of the users password. NTLM uses an encrypted challenge/response protocol to authenticate a user without sending the user password over the wire. Instead, the system requesting authentication must perform a calculation that proves it has access to the secured NTLM credentials.<br></p>',
    );

    $form['miniorange_ldap_kerbeors_desc'] = array(
        '#markup' => '
            <br><h1>What is Kerberos?</h1><hr>
            <p>Kerberos is a client-server authentication protocol that enables mutual authentication –  both the user and the server verify each other’s identity – over non-secure network connections.  The protocol is resistant to eavesdropping and replay attacks, and requires a trusted third party.</p>
            <p>The Kerberos protocol uses a symmetric key derived from the user password to securely exchange a session key for the client and server to use. A server component is known as a Ticket Granting Service (TGS) then issues a security token (AKA Ticket-Granting-Ticket TGT) that can be later used by the client to gain access to different services provided by a Service Server.<br></p><br>',
    );

    $form['save_config_ntlms'] = array(
        '#type' => 'submit',
        '#value' => t('Save Changes'),
        '#disabled' => TRUE,
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
