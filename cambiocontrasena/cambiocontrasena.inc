<?php 

/**
 * Hook_update password
 * @luisjhoham
 * 31.octubre-2012
 * 
 * solicitado para modificar como los usuarios generan una nueva clave
 * formulario de cambio de clave segun core de drupla apoyado en la validacion del core
 */


function cambiocontrasena_form($node, &$form_state) {
global $user;
$id_usuariso = $form_state['build_info']['args'][0];

  //echo ('funciona'. $id_usuariso);
 
  
/*$query = db_select('users', 'u')
   ->condition('uid', $id_usuariso, '=')
   ->limit(1);        
$results = $query->execute();*/


  //$admin = user_access('administer users');


  // Account information.
  $form['account'] = array(
    '#type'   => 'container',
    '#weight' => -10,
  );
  
	$form['account']['pass'] = array(
      '#type' => 'password_confirm', 
      '#size' => 25, 
      '#description' => t('To change the current user password, enter the new password in both fields.'),
    );
    // To skip the current password field, the user must have logged in via a
    // one-time link and have the token in the URL.
    //$pass_reset = isset($_SESSION['pass_reset_' . $account->uid]) && isset($_GET['pass-reset-token']) && ($_GET['pass-reset-token'] == $_SESSION['pass_reset_' . $account->uid]);
    $pass_reset = $id_usuariso;
    $protected_values = array();
    $current_pass_description = '';
    // The user may only change their own password without their current
    // password if they logged in via a one-time login link.
    if (!$pass_reset) {
      $protected_values['mail'] = $form['account']['mail']['#title'];
      $protected_values['pass'] = t('Password');
      $request_new = l(t('Request new password'), 'user/password', array('attributes' => array('title' => t('Request new password via e-mail.'))));
      $current_pass_description = t('Enter your current password to change the %mail or %pass. !request_new.', array('%mail' => $protected_values['mail'], '%pass' => $protected_values['pass'], '!request_new' => $request_new));
    }
    // The user must enter their current password to change to a new one.
      $form['account']['current_pass_required_values'] = array(
        '#type' => 'value', 
        '#value' => $protected_values,
      );
      $form['account']['current_pass'] = array(
        '#type' => 'password', 
        '#title' => t('Current password'), 
        '#size' => 25, 
        '#access' => !empty($protected_values), 
        '#description' => $current_pass_description, 
        '#weight' => -5, 
        '#attributes' => array('autocomplete' => 'off'),
      );
      $form['#validate'][] = '_cambiocontrasena_validate_form';
      $form['actions'] = array('#type' => 'actions');
      $form['actions']['bembele_usuarios_submit'] = array('#type' => 'submit',
        '#value' => t('Guardar'),
       '#prefix' => '<div id="edit-actions">',
			'#suffix' => '</div>',
      '#submit' => array('_cambiocontrasena_pass'),
      );
      
      return $form;
}


 /**
 * Form validacion de campos
 *
 * @see user_account_form()
 */
function _cambiocontrasena_validate_form(&$form, &$form_state) {

}
  
  /**
 * Form validation handler for the current password on the user_account_form().
 *
 * @see user_account_form()
   * funcion que inserta la nueva clave dentro de la base de datos y redirecciona al index
 */
function _cambiocontrasena_pass(&$form, &$form_state) {

 $valorpass= $form_state['values']['pass'];
  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
  $id_usuariso = $form_state['build_info']['args'][0];
  $trmp=user_hash_password($valorpass);
  $clve_base = hash('sha512', $valorpass);
  $clave_final = '$S$'.$clve_base;
  $query = db_update('users')
  ->fields(array(
    'pass' => $trmp,
  ))
  ->condition('uid', $id_usuariso, '=')
  ->execute();
  $form_state['redirect']= "<front>";

}
