<?php

/**
 * @file
 * Documents NAP's hooks for api reference.
 */

/**
 * Act on transaction containing one or more NAP products.
 *
 * @param $nap_purchases
 *   An array of objects, each being a record written to the nap_purchases table.
 * @param $nap_transaction
 *   The transaction object written to the nap_transactions table.
 */
function hook_nap_save_purchase($nap_purchases, $nap_transaction) {
  // no example code
}

/**
 * Plugin to this module to integrate a new e-commerce system.
 *
 * Example code from ec_nap.  See also ec_nap's implementations of hook_enable
 * and hook_disable for how to call nap_notify() to register a nap handler.
 */
function hook_nap_handler() {
  return array(
    // The display name of this module.
    'name' => 'Node Access Product for e-Commerce',
    // The display name of what we're integrating.
    'integration' => 'e-Commerce',
    // The short name of the recurring module (FALSE for none).
    'recurring module' => 'ec_recurring',
    // The parent path of the settings page.
    'settings path' => 'admin/ecsettings',
    // The parent path of the reports page.
    'reports path' => 'admin/store',
  );
}

/**
 * Inform nap that the form is a product form.
 *
 * This hook is invoked of modules that implement hook_nap_handler().
 * Use this to let this module know that the form passed in is a product form
 * and that nap should add it's bits and pieces.
 */
function hook_nap_form_is_product($form, $form_state, $form_id) {
  // Example from ec_product.
  return ec_product_form_is_product($form_id, $form);
}
