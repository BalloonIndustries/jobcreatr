<?php

/**
 * @file
 * Implementation of UUID hooks for all core modules.
 *
 * @todo
 *   Replace all these hook implementations with a generic solution that uses
 *   info from hook_entity_field_info() and hook_entity_property_info(). That
 *   info should contain info about how UUIDs are mapped.
 */

/**
 * @defgroup uuid_property Propery implementations
 * @{
 */

/**
 * Implements hook_entity_uuid_load().
 */
function node_entity_uuid_load(&$entities, $entity_type) {
  if ($entity_type == 'node') {
    entity_property_id_to_uuid($entities, 'user', array('uid', 'revision_uid'));
    entity_property_id_to_uuid($entities, 'node', 'tnid');
  }
}

/**
 * Implements hook_entity_uuid_presave().
 */
function node_entity_uuid_presave(&$entity, $entity_type) {
  if ($entity_type == 'node') {
    entity_property_uuid_to_id($entity, 'user', array('uid', 'revision_uid'));
    entity_property_uuid_to_id($entity, 'node', 'tnid');
  }
}

/**
 * Implements hook_entity_uuid_load().
 */
function book_uuid_entities_features_export_entity_alter(&$entity, $entity_type) {
  if ($entity_type == 'node') {
    if (!empty($entity->book)) {
      $entity->book['bid'] = current(entity_get_uuid_by_id($entity_type, array($entity->book['bid'])));
    }
  }
}

/**
 * Implements hook_entity_uuid_presave().
 */
function book_entity_uuid_presave(&$entity, $entity_type) {
  if ($entity_type == 'node') {
    if (!empty($entity->book)) {
      $entity->book['bid'] = current(entity_get_id_by_uuid($entity_type, array($entity->book['bid'])));
      if (!$entity->book['bid']) {
        $entity->book['bid'] = 'new';
      }
    }
  }
}

/**
 * Implements hook_entity_uuid_presave().
 */
function user_entity_uuid_presave(&$entity, $entity_type) {
  if ($entity_type == 'user') {
    if (!empty($entity->picture)) {
      $uuids = entity_get_id_by_uuid('file', array($entity->picture['uuid']));
      $fid = current($uuids);
      if (!$entity->is_new) {
        $entity->picture = file_load($fid);
      }
      else {
        $entity->picture = $fid;
      }
    }
  }
}

/**
 * Implements hook_entity_uuid_load().
 */
function comment_entity_uuid_load(&$entities, $entity_type) {
  switch ($entity_type) {
    case 'node':
      entity_property_id_to_uuid($entities, 'user', 'last_comment_uid');
      break;

    case 'comment':
      entity_property_id_to_uuid($entities, 'user', array('uid', 'u_uid'));
      entity_property_id_to_uuid($entities, 'node', 'nid');
      break;
  }
}

/**
 * Implements hook_entity_uuid_presave().
 */
function comment_entity_uuid_presave(&$entity, $entity_type) {
  switch ($entity_type) {
    case 'node':
      entity_property_uuid_to_id($entity, 'user', 'last_comment_uid');
      break;

    case 'comment':
      entity_property_uuid_to_id($entity, 'user', array('uid', 'u_uid'));
      entity_property_uuid_to_id($entity, 'node', 'nid');
      break;
  }
}

/**
 * Implements hook_entity_uuid_load().
 */
function file_entity_uuid_load(&$entities, $entity_type) {
  if ($entity_type == 'file') {
    entity_property_id_to_uuid($entities, 'user', 'uid');
  }
}

/**
 * Implements hook_entity_uuid_presave().
 */
function file_entity_uuid_presave(&$entity, $entity_type) {
  if ($entity_type == 'file') {
    entity_property_uuid_to_id($entity, 'user', 'uid');

    // Write the new file to the local filesystem.
    if (isset($entity->file_contents)) {
      // Don't try to write it if it uses a stream wrapper that isn't writeable
      // (for example, if it is a remotely-hosted video).
      $scheme = file_uri_scheme($entity->uri);
      $wrappers = file_get_stream_wrappers(STREAM_WRAPPERS_WRITE);
      if (empty($wrappers[$scheme])) {
        return;
      }

      // Check for an existing file with the same URI.
      $existing_files = file_load_multiple(array(), array('uri' => $entity->uri));
      $existing = (object) array('uri' => NULL, 'uuid' => NULL);
      if (count($existing_files)) {
        $existing = reset($existing_files);
      }

      // If this is a new file and there is an existing file with the same URI,
      // but a different uuid then rename this file.
      if ($entity->is_new && $entity->uri == $existing->uri && $entity->uuid != $existing->uuid) {
        $uri = $entity->uri;
        $replace = FILE_EXISTS_RENAME;
      }
      // If this has an id, meaning UUID has already matched the uuid to an
      // existing file, but it has a URI that matches a file with a different
      // uuid, then load the file with the matching uuid and use the URI from
      // that file. The existing file with the matching uuid is most likely a
      // file that was previously renamed, e.g. as in the condition above, to
      // avoid conflict. The uuid matches because they are the same file, but
      // the URI does not because an incrementing number was added as part of
      // the renaming.
      elseif ($entity->uri == $existing->uri && $entity->uuid != $existing->uuid) {
        $file = file_load($entity->fid);
        $uri = $file->uri;
        $replace = FILE_EXISTS_REPLACE;
      }
      // Otherwise create a new file or replace the existing file contents.
      else {
        $uri = $entity->uri;
        $replace = FILE_EXISTS_REPLACE;
      }

      $directory = drupal_dirname($uri);
      file_prepare_directory($directory, FILE_CREATE_DIRECTORY);
      $entity->uri = file_unmanaged_save_data(base64_decode($entity->file_contents), $uri, $replace);
    }
  }
}

/**
 * Implements hook_entity_uuid_load().
 */
function taxonomy_entity_uuid_load(&$entities, $entity_type) {
  if ($entity_type == 'taxonomy_term') {
    foreach ($entities as &$entity) {
      if (isset($entity->parent)) {
        if (!is_array($entity->parent)) {
          $entity->parent = array($entity->parent);
        }
        $uuids = entity_get_uuid_by_id('taxonomy_term', $entity->parent);
        $entity->parent = array_values($uuids);
      }
      unset($entity->vid);
    }
  }
}

/**
 * Implements hook_entity_uuid_presave().
 */
function taxonomy_entity_uuid_presave(&$entity, $entity_type) {
  if ($entity_type == 'taxonomy_term') {
    if (isset($entity->parent)) {
      if (!is_array($entity->parent)) {
        $entity->parent = array($entity->parent);
      }
      $ids = entity_get_id_by_uuid('taxonomy_term', $entity->parent);
      $entity->parent = array_values($ids);
    }
    $vocabulary = taxonomy_vocabulary_machine_name_load($entity->vocabulary_machine_name);
    $entity->vid = $vocabulary->vid;
  }
}

/**
 * Implements hook_entity_uuid_load().
 */
function field_entity_uuid_load(&$entities, $entity_type) {
  foreach ($entities as $i => $entity) {
    list(, , $bundle_name) = entity_extract_ids($entity_type, $entity);
    $instances = field_info_instances($entity_type, $bundle_name);

    foreach ($instances as $field_name => $instance) {
      $field = field_info_field($field_name);
      if (!empty($field) && isset($entity->{$field_name})) {
        foreach ($entity->{$field_name} as $langcode => &$items) {
          // Invoke 'hook_field_uuid_load'. We can't use module_invoke() since
          // that is not passing by reference.
          $function = $field['module'] . '_field_uuid_load';
          if (function_exists($function)) {
            $function($entity_type, $entity, $field, $instance, $langcode, $items);
          }
        }
      }
    }
  }
}

/**
 * Implements hook_entity_uuid_presave().
 */
function field_entity_uuid_presave(&$entity, $entity_type) {
  list(, , $bundle_name) = entity_extract_ids($entity_type, $entity);
  $instances = field_info_instances($entity_type, $bundle_name);

  foreach ($instances as $field_name => $instance) {
    $field = field_info_field($field_name);
    if (!empty($field) && isset($entity->{$field_name})) {
      foreach ($entity->{$field_name} as $langcode => &$items) {
        // Invoke 'hook_field_uuid_load'. We can't use module_invoke() since
        // that is not passing by reference.
        $function = $field['module'] . '_field_uuid_presave';
        if (function_exists($function)) {
          $function($entity_type, $entity, $field, $instance, $langcode, $items);
        }
      }
    }
  }
}

/**
 * @} End of "Property implementations"
 */

/**
 * @defgroup uuid_field Field implementations
 * @{
 */

/**
 * Implements hook_field_uuid_load().
 */
function taxonomy_field_uuid_load($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_id_to_uuid($items, 'taxonomy_term', 'tid');
}

/**
 * Implements hook_field_uuid_presave().
 */
function taxonomy_field_uuid_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_uuid_to_id($items, 'taxonomy_term', 'tid');
}

/**
 * Implements hook_field_uuid_load().
 */
function file_field_uuid_load($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_id_to_uuid($items, 'file', 'fid');
  entity_property_id_to_uuid($items, 'user', 'uid');
}

/**
 * Implements hook_field_uuid_presave().
 */
function file_field_uuid_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_uuid_to_id($items, 'file', 'fid');
  entity_property_uuid_to_id($items, 'user', 'uid');
}

/**
 * Implements hook_field_uuid_load().
 */
function image_field_uuid_load($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_uuid_load($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_uuid_presave().
 */
function image_field_uuid_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_uuid_presave($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_uuid_load().
 */
function node_reference_field_uuid_load($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_id_to_uuid($items, 'node', 'nid');
}

/**
 * Implements hook_field_uuid_presave().
 */
function node_reference_field_uuid_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_uuid_to_id($items, 'node', 'nid');
}

/**
 * Implements hook_field_uuid_load().
 */
function user_reference_field_uuid_load($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_id_to_uuid($items, 'user', 'uid');
}

/**
 * Implements hook_field_uuid_presave().
 */
function user_reference_field_uuid_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_uuid_to_id($items, 'user', 'uid');
}

/**
 * Implements hook_field_uuid_load().
 */
function entityreference_field_uuid_load($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // TODO: This is not really good, but as of now 'entity_property_id_to_uuid()'
  // can't handle a single $item.
  entity_property_id_to_uuid($items, $field['settings']['target_type'], 'target_id');
}

/**
 * Implements hook_field_uuid_presave().
 */
function entityreference_field_uuid_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // TODO: This is not really good, but as of now 'entity_property_id_to_uuid()'
  // can't handle a single $item.
  entity_property_uuid_to_id($items, $field['settings']['target_type'], 'target_id');
}

/**
 * Implements hook_entity_uuid_load().
 */
function field_collection_entity_uuid_load(&$entities, $entity_type) {
  if ($entity_type == 'field_collection_item') {
    entity_property_id_to_uuid($entities, 'field_collection_item', 'value');
  }
}

/**
 * Implements hook_entity_uuid_presave().
 */
function field_collection_entity_uuid_presave(&$entity, $entity_type) {
  if ($entity_type == 'field_collection_item') {
    entity_property_uuid_to_id($entity, 'field_collection_item', 'value');
  }
}

/**
 * Implements hook_field_uuid_load().
 */
function field_collection_field_uuid_load($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_id_to_uuid($items, 'field_collection_item', 'value');
}

/**
 * Implements hook_field_uuid_presave().
 */
function field_collection_field_uuid_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  entity_property_uuid_to_id($items, 'field_collection_item', 'value');
}

/**
 * @} End of "Field implementations"
 */

/**
 * @defgroup uuid_export Export alterations
 * @{
 */

/**
 * Implements hook_uuid_entities_features_export_entity_alter().
 */
function node_uuid_entities_features_export_entity_alter(&$entity, $entity_type) {
  if ($entity_type == 'node') {
    foreach (array('data', 'name', 'picture', 'revision_uid', 'last_comment_timestamp') as $property) {
      if (property_exists($entity, $property)) {
        unset($entity->{$property});
      }
    }
  }
}

/**
 * Implementations hook_uuid_entities_features_export_entity_alter().
 */
function user_uuid_entities_features_export_entity_alter(&$entity, $entity_type) {
  if ($entity_type == 'user') {
    foreach (array('data', 'access', 'login') as $property) {
      if (property_exists($entity, $property)) {
        unset($entity->{$property});
      }
    }
  }
}

/**
 * Implements hook_uuid_entities_features_export_alter().
 */
function file_uuid_entities_features_export_field_alter($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as &$item) {
    if (isset($item['timestamp'])) {
      unset($item['timestamp']);
    }
  }
}

/**
 * Implements hook_uuid_entities_features_export_entity_alter().
 */
function workbench_uuid_entities_features_export_entity_alter(&$entity, $entity_type) {
  foreach (array('workbench_moderation', 'my_revision', 'workbench_access', 'workbench_access_scheme', 'workbench_access_by_role') as $property) {
    if (isset($entity->{$property})) {
      unset($entity->{$property});
    }
  }
}

/**
 * @} End of "Export alterations"
 */
