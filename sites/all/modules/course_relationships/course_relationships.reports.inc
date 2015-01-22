<?php

/**
 * @file
 * Custom reports for course relationships.
 */

/**
 * Make a report of all children drilled down to a certain level to show completions.
 */
function course_relationships_activity_set_report_tree($node, $toLevel = 2) {
  $parent = $node->nid;

  // Rows that will get output to a table/csv.
  $rows = array();

  $user_fields = array(
    'uid' => 'User ID',
    'name' => 'Name',
  );

  $levelParents = array($parent => $parent);
  for ($level = 1; $level <= $toLevel; $level++) {
    $placeholders = db_placeholders($levelParents);
    $sql = "SELECT co.*, co.title, co.instance AS child FROM {node} np
    INNER JOIN {course_outline} co ON (co.nid = np.nid)
    INNER JOIN {node} n ON co.nid = n.nid
    WHERE co.nid IN (:nids)
    ORDER BY co.nid, co.weight";
    $result = db_query($sql, array(':nids' => array_keys($levelParents)));
    $levelParents = array();
    foreach ($user_fields as $key => $label) {
      $levelParents[$key] = '';
    }
    while ($row = $result->fetchObject()) {
      $levelParents[$row->child] = array(
        'data' => $row->title,
        'colspan' => 0,
      );
      if ($rows[$level - 1]) {
        $rows[$level - 1][$row->nid]['colspan']++;
      }
    }
    $rows[$level] = $levelParents;
  }

  $final_row = & $rows[$level - 1];
  foreach ($user_fields as $key => $label) {
    $final_row[$key] = $label;
  }

  $placeholders = db_placeholders($final_row);
  $sql = "SELECT * FROM {course_report} cr
  LEFT JOIN {users} u ON cr.uid = u.uid
  WHERE nid IN (:nids)";
  $result = db_query($sql, array(':nids' => array_keys($final_row)));
  $users = array();
  while ($row = $result->fetchObject) {
    $users[$row->uid][$row->nid] = $row;
  }

  foreach ($users as $uid => $user) {
    $row = array();
    foreach ($final_row as $child => $data) {
      if (!is_numeric($child)) {
        $row[] = reset($user)->{$child};
      }
      else {
        $row[] = $user[$child]->section_name;
      }
    }
    $rows[] = $row;
  }

  $headers = array();

  if (isset($_GET['csv']) && function_exists('theme_format_csv')) {
    header("Content-type: text/csv");
    header("Content-disposition: attachment; filename=$parent.csv");
    print theme_format_csv($headers, $rows);
    exit;
  }
  else {
    return theme_table($headers, $rows) .
      l('Download CSV', $_GET['q'], array('query' => 'csv'));
  }
}
