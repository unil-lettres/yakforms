<?php
function ldap_auth_license($form, $form_state)
{
    global $base_url;

  $form['miniorange_ldap_license_tab'] = array(
    '#markup' => "
    <head>
        <link href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO' crossorigin='anonymous'>
    </head>
    <div id='mo_oauth_table_layout'>
        <p><strong style='color: red'>Note: </strong>If you want to test any of our premium feature, you can request a demo by dropping us an email at <a href='mailto:drupalsupport@xecurify.com'>drupalsupport@xecurify.com</a></p>
		<h2>Licensing Plans<a style='float:right' target='_blank' href='https://plugins.miniorange.com/drupal-ldap' class='btn btn-primary' role='button'>Know more</a></h2><br>
		<table class='table mo_table-bordered mo_table-striped' border='1' bordercolor='black'>
            <thead class='thead-dark'>
            <tr style='background-color:#FFFFFF'>
                <th width='20%' style='text-align:center'><br>
                    <h3><font color='#FFFFFF'>Features \ Plans</font></h3><br><br></th>
                <th width='10%' style='text-align:center'><br></h3><p class='mo_plan-desc'></p><h3><font color='#FFFFFF'>FREE</font><br><br><br><span>
                </span></h3></th>
                <th style='text-align:center' width='10%'><h3><font color='#FFFFFF'>Standard<br></font></h3><p class='mo_plan-desc'></p><h3><font color='#FFFFFF'>$249<sup>*</sup></font><br></h3><span>One Time Payment</span><h3><br><span>
                <a href='https://portal.miniorange.com/initializePayment?requestOrigin=drupal_ldap_standard_plan' class='btn btn-primary' style='margin-bottom:3.8%;' role='button' target='_blank' id='oauth_upgrade'>Click to Upgrade</a>
                </span></h3></th>
                <th style='text-align:center' width='10%'><h3><font color='#FFFFFF'>Premium</font></h3><p></p><p class='mo_plan-desc'></p><h3><font color='#FFFFFF'>$399<sup>*</sup></font><br></h3><span>One Time Payment</span><h3><br><span>
				<a href='https://portal.miniorange.com/initializePayment?requestOrigin=drupal_ldap_premium_plan' class='btn btn-primary' style='margin-bottom:3.8%;' role='button' target='_blank' id='oauth_upgrade'>Click to Upgrade</a>
            </th>
			</tr>
            </thead>
			 <tbody>
			<tr>
                <td style='text-align:center'>Unlimited Authentications</td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>&#x2714;</td>
            </tr>
			<tr>
            <td style='text-align:center'>Single LDAP Directory Configuration </td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>&#x2714;</td>
            </tr>
            <tr>
                <td style='text-align:center'>Search User by Single Attribute</td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>Custom Search Filter</td>
            </tr>
            <tr>
                <td style='text-align:center'>Single Search Base</td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>Multiple Search Bases</td>
            </tr>
            <tr>
                <td style='text-align:center'>Attribute Mapping</td>
				<td style='text-align:center'>Username Mapping</td>
                <td style='text-align:center'>Basic Attribute Mapping</td>
                <td style='text-align:center'>Custom Attribute Mapping</td>
            </tr>
            <tr>
			<td style='text-align:center'>Auto Create Users</td>
                <td style='text-align:center'></td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>&#x2714;</td>
            </tr>
			<tr>
            <td style='text-align:center'>Role Mapping </td>
                <td style='text-align:center'></td>
                <td style='text-align:center'>Basic</td>
                <td style='text-align:center'>Advanced</td>
            </tr>
			<tr>
                <td style='text-align:center'>TLS Connection</td>
                <td style='text-align:center'></td>
                <td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>&#x2714;</td>
            </tr>
			 <tr>
                <td style='text-align:center'>Drupal to LDAP updates on user profile</td>
                <td style='text-align:center'></td>
                <td style='text-align:center'></td>
                <td style='text-align:center'>&#x2714;</td>
            </tr>
            <tr>
                <td style='text-align:center'>NTLM & Kerberos Authentication</td>
                <td style='text-align:center'></td>
                <td style='text-align:center'></td>
                <td style='text-align:center'>&#x2714;</td>
            </tr>
            <tr>
                <td style='text-align:center'>Support for Custom Integration</td>
                <td style='text-align:center'></td>
				<td style='text-align:center'>&#x2714;</td>
                <td style='text-align:center'>&#x2714;</td>
            </tr>
		</table>
        *Cost applicable for one instance only. Licenses are perpetual and the support plan includes support and 12 months of free maintenance(version updates). You can renew maintenance after 12 months at 50% of the current license cost.
        <br>
        <br><b>10 Days Return Policy -</b><br><br>
    At miniOrange, we want to ensure that you are 100% happy with your purchase. If the module you purchased is not working as advertised and you have attempted to resolve any issues with our support team, which could not get resolved, we will refund the whole amount given that you raised a request for refund within the first 10 days of the purchase. Please email us at <a href='mailto:drupalsupport@xecurify.com'>drupalsupport@xecurify.com</a> for any queries regarding the return policy.
		</div> ");
    return $form;
}
?>
