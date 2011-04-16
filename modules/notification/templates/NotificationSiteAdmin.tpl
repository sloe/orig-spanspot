{*
 * $Revision: 847 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<div class="gbBlock gcBackground1">
    <h2> {g->text text="Site Notification Administration"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock">
    <h2 class="giSuccess"> {g->text text="Settings saved successfully"} </h2>
</div>
{/if}

{* Methods *}
<div class="gbBlock">
    <table class="gbDataTable">
	<tr>
	    <th colspan="3"> {g->text text="Methods"} </th>
	</tr>
    {foreach from=$NotificationMethods item=method name=methods}
	{if $smarty.foreach.methods.iteration % 30 == 1}
	<tr>
	    <th> {g->text text="Name"} </th>
	    <th> {g->text text="Description"} </th>
	    <th> {g->text text="Enabled"} </th>
	</tr>
	{/if}
	<tr>
	    <td>{$method.methodName}</td>
	    <td>{$method.methodDescription}</td>
	    <td><input type="checkbox" 
		name="{g->formVar var="form[methods][`$method.methodClass`]"}"
		{if !empty($method.enabled)} checked="checked"{/if} /></td>
	</tr>
    {/foreach}
	<tr>
	    <td colspan="3"> &nbsp; </td>
	</tr>
{* Events *}
	<tr>
	    <th colspan="3"> {g->text text="Events"} </th>
	</tr>
    {foreach from=$NotificationEvents item=event name=events}
	{if $smarty.foreach.events.iteration % 30 == 1}
	<tr>
	    <th> {g->text text="Name"} </th>
	    <th> {g->text text="Description"} </th>
	    <th> {g->text text="Enabled"} </th>
	</tr>
	{/if}
	<tr>
	    <td>{$event.eventName}</td>
	    <td>{$event.eventDescription}</td>
	    <td><input type="checkbox" name="{g->formVar var="form[events][`$event.eventClass`]"}"
		{if !empty($event.enabled)} checked="checked"{/if} /></td>
	</tr>
    {/foreach}
    </table>
</div>

<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][save]"}" 
	value="{g->text text="Save"}" />
    <input type="reset" class="inputTypeSubmit" value="{g->text text="Reset"}"/>
</div>
